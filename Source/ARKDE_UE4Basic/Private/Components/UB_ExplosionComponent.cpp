// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_ExplosionComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UUB_ExplosionComponent::UUB_ExplosionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bTakeEnvironmentIntoAccount = true;
	bHasFalloffExplosion = true;

	ExplosionMaxDamage = 30.0f;
	ExplosionMinDamage = 15.0f;
	ExplosionInnerRadius = 50.0f;
	ExplosionOuterRadius = 80.0f;

	ExplosionDelay = 3.0f;
}

// Called when the game starts
void UUB_ExplosionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UUB_ExplosionComponent::StartExplosion(const TArray<AActor*> Ignore)
{
	IgnoreActors = Ignore;
	GetWorld()->GetTimerManager().SetTimer(ExplosionDelayTimer, this, &UUB_ExplosionComponent::Explode, ExplosionDelay, false);
}

void UUB_ExplosionComponent::Explode()
{
	GetWorld()->GetTimerManager().ClearTimer(ExplosionDelayTimer);

	if (ExplosionInnerRadius >= ExplosionOuterRadius) UE_LOG(LogTemp, Error, TEXT("ExplosionInnerRadius is NOT less than ExplosionOuterRadius"));
	if (ExplosionMaxDamage < ExplosionMinDamage) UE_LOG(LogTemp, Error, TEXT("ExplosionMaxDamage is NOT bigger than ExplosionMinDamage"));

	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	const FVector ExplosionCenter = Owner->GetActorLocation();
	if (IsValid(Owner)) {

		if (!bTakeEnvironmentIntoAccount) { //then use sphere overlap

			const TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes = {
				UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn), //Collide with pawn
				//UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic),
			};

			TArray<AActor*> OutActors;
			bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), ExplosionCenter, ExplosionOuterRadius, traceObjectTypes, nullptr, IgnoreActors, OutActors);

			if (bHit) {
				for (AActor* overlappedActor : OutActors) {
					if (!IsValid(overlappedActor)) continue;

					//Verify how close is to the center of the explosion
					float Distance = (ExplosionCenter - overlappedActor->GetActorLocation()).Size();
					float DamageToApply;

					if (Distance <= ExplosionInnerRadius) {
						DamageToApply = ExplosionMaxDamage;
					}
					else if (Distance >= ExplosionOuterRadius) {
						DamageToApply = ExplosionMinDamage;
					}
					else { //calculate de damage between explosionInnerRadius and explosionOuterRadius
						float InsideRadiusRange = ExplosionOuterRadius - ExplosionInnerRadius;

						//distance in the range: 0 start, 1 end, this in order to lerp
						float NormalizedDistance = ((Distance - ExplosionInnerRadius) / InsideRadiusRange);
						DamageToApply = FMath::Lerp(ExplosionMaxDamage, ExplosionMinDamage, NormalizedDistance); //this is just a linear interpolation for calculate the damage
						UE_LOG(LogTemp, Log, TEXT("Normalized Distance %f"), NormalizedDistance);
					}

					UE_LOG(LogTemp, Log, TEXT("Distance %f"), Distance);
					UGameplayStatics::ApplyDamage(overlappedActor, DamageToApply, Owner->GetInstigatorController(), Owner, DamageType);
				}
			}

			OnExplodeDelegate.Broadcast(this, OutActors);
		}
		else {
			float DamageFalloff = 1.0f;

			if (IsValid(DamageType)) {
				UDamageType* CurrentDamageType = DamageType->GetDefaultObject<UDamageType>();
				if (IsValid(CurrentDamageType)) {
					DamageFalloff = CurrentDamageType->DamageFalloff;
				}
			}

			bool bAppliedDamage = false;
			if (bHasFalloffExplosion) {
				bAppliedDamage = UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionMaxDamage, ExplosionCenter, ExplosionOuterRadius, DamageType,
					IgnoreActors, Owner, Owner->GetInstigatorController(), true);
			}
			else {
				bAppliedDamage = UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), ExplosionMaxDamage, ExplosionMinDamage, ExplosionCenter,
					ExplosionInnerRadius, ExplosionOuterRadius, DamageFalloff, DamageType,
					IgnoreActors, Owner, Owner->GetInstigatorController());
			}

			UE_LOG(LogTemp, Log, TEXT("Applied damage %s"), bAppliedDamage ? TEXT("TRUE") : TEXT("FALSE"));

			//TODO, send damaged actors, HOW to get them?
			TArray<AActor*> OutActors;
			OnExplodeDelegate.Broadcast(this, OutActors);
		}
	}

	if (bDebugExplosion) {
		DrawDebugSphere(GetWorld(), ExplosionCenter, ExplosionInnerRadius, 20, FColor::Yellow, false, 5.0f, 0, 1.0f);
		DrawDebugSphere(GetWorld(), ExplosionCenter, ExplosionOuterRadius, 20, FColor::White, false, 5.0f, 0, 1.0f);
	}

	if (IsValid(ExplosionEffect)) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, ExplosionCenter, Owner->GetActorRotation());
	}
}


