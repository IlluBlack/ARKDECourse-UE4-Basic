// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Ult_HighlightEnemies.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ARKDE_UE4Basic.h"
#include "Engine/World.h"

//#include "..\..\Public\Ultimates\UB_Ult_HighlightEnemies.h"

AUB_Ult_HighlightEnemies::AUB_Ult_HighlightEnemies()
{
	DetectionRadius = 20.0f;
}

void AUB_Ult_HighlightEnemies::StartUltimateBehavior()
{
	Super::StartUltimateBehavior();
	DetectEnemies();
}

void AUB_Ult_HighlightEnemies::DetectEnemies()
{
	int NumberOfEnemies = 0;

	const FVector OwnerCenter = GetOwner()->GetActorLocation();
	const TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes = {
				UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn), //Collide with pawn
	};
	const TArray<AActor*> IgnoreActors = { GetOwner() };
	TArray<AActor*> OutActors;

	bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), OwnerCenter, DetectionRadius, traceObjectTypes, nullptr, IgnoreActors, OutActors);

	if (bHit) {
		for (AActor* overlappedActor : OutActors) 
		{
			if (!IsValid(overlappedActor)) continue;

			if (overlappedActor->ActorHasTag(TAG_ENEMY)) {
				NumberOfEnemies += 1;
			}
		}
	}

	if (bDebugDetectionRadius) {
		DrawDebugSphere(GetWorld(), OwnerCenter, DetectionRadius, 20, FColor::Cyan, false, 1.0f);
	}

	//TODO: Display this in UI
	BP_DisplayDetectedEnemies(NumberOfEnemies);
}
