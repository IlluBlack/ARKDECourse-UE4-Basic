// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_BotPetControlBase.h"
#include "UB_Character.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Classes/Engine/World.h"

// Sets default values
AUB_BotPetControlBase::AUB_BotPetControlBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ControlRadius = 1000.0f;
}

// Called when the game starts or when spawned
void AUB_BotPetControlBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (bDebug) {
		DrawDebugSphere(GetWorld(), GetActorLocation(), ControlRadius, 25, FColor::White, true, 5.0f, 0, 2.0f);
	}
}

// Called every frame
void AUB_BotPetControlBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUB_BotPetControlBase::SuscribeBotPet(AUB_BotPet* BotPetToControl)
{
	BotPetsControlled.Add(BotPetToControl);

	if (BotPetsControlled.Num() > RestSeats.Num()) {
		UE_LOG(LogTemp, Error, TEXT("BotPetControlBase has no enough RestSeats for BotPetsControlled"));
	}
}

FVector AUB_BotPetControlBase::GetLocationRestSeat(AUB_BotPet* BotPet)
{
	int indexBotPet;
	if (BotPetsControlled.Find(BotPet, indexBotPet)) {
		if (indexBotPet < RestSeats.Num()) {
			return (GetActorLocation() + RestSeats[indexBotPet]); //each bot has its own restSeat
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("BotPetControlBase has no enough RestSeats for BotPetsControlled"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("This BotPet does not belong to this ControlBase"));
	}

	return GetActorLocation();
}

TArray<AUB_Character*> AUB_BotPetControlBase::GetCharactersInControlRadius()
{
	const TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes = {
				UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn), //Detect Pawns
	};
	const TArray<AActor*> IgnoreActors = {};

	TArray<AActor*> OutActors;
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ControlRadius, traceObjectTypes, nullptr, IgnoreActors, OutActors);

	TArray<AUB_Character*> CharactersInRadius;
	if (bHit) {
		for (AActor* OverlappedActor : OutActors) 
		{
			if (!IsValid(OverlappedActor)) continue;

			AUB_Character* OverlappedCharacter = Cast<AUB_Character>(OverlappedActor);
			if (!IsValid(OverlappedActor)) continue;
			if (OverlappedCharacter->IsDead()) continue;

			CharactersInRadius.Add(OverlappedCharacter);
		}
	}

	return CharactersInRadius;
}

