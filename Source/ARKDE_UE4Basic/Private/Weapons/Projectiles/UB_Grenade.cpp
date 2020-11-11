// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Grenade.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"

#include "Kismet/KismetSystemLibrary.h"

AUB_Grenade::AUB_Grenade()
{
	bTakeEnvironmentIntoAccount = true;
	bHasFalloffExplosion = true;

	ExplosionMaxDamage = 30.0f;
	ExplosionMinDamage = 15.0f;
	ExplosionInnerRadius = 50.0f;
	ExplosionOuterRadius = 80.0f;

	ExplosionDelay = 3.0f;
	DisappearDelay = 1.0f;
}

void AUB_Grenade::Throw()
{
	GetWorldTimerManager().SetTimer(ExplosionDelayTimer, this, &AUB_Grenade::Explode, ExplosionDelay, false);
}

void AUB_Grenade::Explode()
{
	GetWorldTimerManager().ClearTimer(ExplosionDelayTimer);

	if (ExplosionInnerRadius >= ExplosionOuterRadius) UE_LOG(LogTemp, Error, TEXT("ExplosionInnerRadius is NOT less than ExplosionOuterRadius"));
	if (ExplosionMaxDamage < ExplosionMinDamage) UE_LOG(LogTemp, Error, TEXT("ExplosionMaxDamage is NOT bigger than ExplosionMinDamage"));

	FVector ExplosionCenter = GetActorLocation();
	const TArray<AActor*> IgnoreActors = {}; //at this moment 

	AActor* Owner = GetOwner();
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
					UGameplayStatics::ApplyDamage(overlappedActor, DamageToApply, Owner->GetInstigatorController(), this, DamageType);
				}
			}

		}
		else {

			UDamageType* CurrentDamageType = DamageType->GetDefaultObject<UDamageType>();

			if (IsValid(CurrentDamageType)) {
				bool bAppliedDamage = false;

				if (bHasFalloffExplosion) {
					bAppliedDamage = UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionMaxDamage, ExplosionCenter, ExplosionOuterRadius, DamageType,
						IgnoreActors, this, Owner->GetInstigatorController());
				}
				else {
					bAppliedDamage = UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), ExplosionMaxDamage, ExplosionMinDamage, ExplosionCenter,
						ExplosionInnerRadius, ExplosionOuterRadius, CurrentDamageType->DamageFalloff, DamageType,
						IgnoreActors, this, Owner->GetInstigatorController());
				}

				UE_LOG(LogTemp, Log, TEXT("Applied damage %s"), bAppliedDamage ? TEXT("TRUE") : TEXT("FALSE"));
			}

		}

	}

	if (bDebugExplosion) {
		DrawDebugSphere(GetWorld(), ExplosionCenter, ExplosionInnerRadius, 20, FColor::Yellow, false, 1.0f);
		DrawDebugSphere(GetWorld(), ExplosionCenter, ExplosionOuterRadius, 20, FColor::White, false, 1.0f);
	}

	if (IsValid(ExplosionEffect)) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, ExplosionCenter, GetActorRotation());
	}

	GetWorldTimerManager().SetTimer(DisappearDelayTimer, this, &AUB_Grenade::Disappear, DisappearDelay, false);
}


void AUB_Grenade::Disappear()
{
	GetWorldTimerManager().ClearTimer(DisappearDelayTimer);
	Destroy();
}