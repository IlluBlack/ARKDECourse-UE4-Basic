// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Grenade.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"

AUB_Grenade::AUB_Grenade()
{
	bHasFalloffExplosion = true;

	ExplosionBaseDamage = 30.0f;
	ExplosionMinDamage = 15.0f;
	ExplosionInnerRadius = 20.0f;
	ExplosionOuterRadius = 30.0f;

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

	//TArray<FHitResult> HitsResult;
	//FCollisionShape ExplosionCollision = FCollisionShape::MakeSphere(ExplosionRadius);

	FVector Location = GetActorLocation();
	TArray<AActor*> IgnoreActors = { };

	AActor* Owner = GetOwner();
	if (IsValid(Owner)) {
		UDamageType* CurrentDamageType = DamageType->GetDefaultObject<UDamageType>();

		if (IsValid(CurrentDamageType)) {
			bool bAppliedDamage = false;

			if (bHasFalloffExplosion) {
				bAppliedDamage = UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionBaseDamage, Location, ExplosionInnerRadius + ExplosionOuterRadius,
					DamageType, IgnoreActors, this, Owner->GetInstigatorController());
			}
			else {
				bAppliedDamage = UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), ExplosionBaseDamage, ExplosionMinDamage, Location,
					ExplosionInnerRadius, ExplosionOuterRadius, CurrentDamageType->DamageFalloff, DamageType,
					IgnoreActors, this, Owner->GetInstigatorController());
			}

			UE_LOG(LogTemp, Log, TEXT("Applied damage %s"), bAppliedDamage? TEXT("TRUE"):TEXT("FALSE"));
		}
	}

	if (bDebugExplosion) {
		DrawDebugSphere(GetWorld(), Location, ExplosionInnerRadius, 20, FColor::Yellow, false, 1.0f);
		DrawDebugSphere(GetWorld(), Location, ExplosionInnerRadius + ExplosionOuterRadius, 20, FColor::White, false, 1.0f);
	}

	if (IsValid(ExplosionEffect)) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, Location, GetActorRotation());
	}

	GetWorldTimerManager().SetTimer(DisappearDelayTimer, this, &AUB_Grenade::Disappear, DisappearDelay, false);
}


void AUB_Grenade::Disappear()
{
	GetWorldTimerManager().ClearTimer(DisappearDelayTimer);
	Destroy();
}