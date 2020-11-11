// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Rifle.h"
#include "ARKDE_UE4Basic.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"

AUB_Rifle::AUB_Rifle()
{
	TraceLenght = 10000.0f;
}

void AUB_Rifle::Fire()
{
	Super::Fire();

	AActor* Owner = GetOwner();
	if (IsValid(Owner)) {
		FHitResult HitResult;

		FVector EyeLocation;
		FRotator EyeRotation;
		Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector(); //it's the lookDirection
		FVector TraceEnd = EyeLocation + (ShotDirection * TraceLenght);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); //ignore my weapon collision
		QueryParams.AddIgnoredActor(Owner); //ignore my owner collision
		QueryParams.bTraceComplex = true; //to be able to get more data, and more accurate collision

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, TCC_WEAPON, QueryParams);

		FVector ResultTraceEnd = TraceEnd; //By default is the final point of the lineTrace if not collision

		if (bHit) {
			AActor* HitActor = HitResult.GetActor();
			if (IsValid(HitActor)) {
				UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, Owner->GetInstigatorController(), this, DamageType);
				UE_LOG(LogTemp, Log, TEXT("Hit another actor"));
			}

			ResultTraceEnd = HitResult.ImpactPoint;
			//Play ImpactEffect
			if (IsValid(ImpactEffect)) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ResultTraceEnd, HitResult.ImpactNormal.Rotation());
			}
		}

		if (bDebugLineTrace) {
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0.0f, 1.0f);
		}

		PlayMuzzleEffect();

		//Play TraceEffect
		if (IsValid(TraceEffect)) {
			UParticleSystemComponent* TraceComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, GetMuzzleSocketLocation());

			if (IsValid(TraceComponent)) {
				TraceComponent->SetVectorParameter(TraceEndParamName, ResultTraceEnd);
			}
		}
	}
}