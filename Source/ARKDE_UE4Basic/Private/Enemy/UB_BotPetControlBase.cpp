// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_BotPetControlBase.h"
#include "UB_Character.h"
#include "Enemy/UB_BotPet.h"

#include "Components/BillboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"

// Sets default values
AUB_BotPetControlBase::AUB_BotPetControlBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Spawner Billboard"));
	RootComponent = SpawnerBillboardComponent;

	bIsActive = true;
	MaxBotsCounter = 1;
	SpawnFrequency = 1.0f;

	ControlRadius = 1000.0f;
	SpawnLootProbability = 100.0f;
}

// Called when the game starts or when spawned
void AUB_BotPetControlBase::BeginPlay()
{
	Super::BeginPlay();

	if (bIsActive) ActivateSpawner();
	
	if (bDebug) {
		DrawDebugSphere(GetWorld(), GetActorLocation(), ControlRadius, 25, FColor::White, true, 5.0f, 0, 2.0f);
	}
}

void AUB_BotPetControlBase::ActivateSpawner()
{
	if (bIsActive) return;
	bIsActive = true;
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot, this, &AUB_BotPetControlBase::SpawnBot, SpawnFrequency, true);
}

void AUB_BotPetControlBase::DeactivateSpawner()
{
	if (!bIsActive) return;
	bIsActive = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnBot);
}

void AUB_BotPetControlBase::SpawnBot()
{
	if (!bIsActive) return;
	if (CurrentBotsCounter >= MaxBotsCounter) return;
	if (IsValid(BotPetClass)) {
		FVector SpawnPoint = GetSpawnPoint();
		//FActorSpawnParameters SpawnParameters;
		//SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//AUB_BotPet* NewBot = GetWorld()->SpawnActor<AUB_BotPet>(BotPetClass, SpawnPoint, FRotator::ZeroRotator, SpawnParameters);

		FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnPoint);
		AUB_BotPet* NewBot = GetWorld()->SpawnActorDeferred<AUB_BotPet>(BotPetClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding);
		SuscribeBotPet(NewBot);
		NewBot->FinishSpawning(SpawnTransform);
	}
}

FVector AUB_BotPetControlBase::GetSpawnPoint()
{
	if (SpawnPoints.Num() > 0) {
		int RandomIndex = FMath::RandRange(0, SpawnPoints.Num()-1);
		return UKismetMathLibrary::TransformLocation(GetActorTransform(), SpawnPoints[RandomIndex]);
	}
	else{
		return GetActorLocation();
	}
}

void AUB_BotPetControlBase::SuscribeBotPet(AUB_BotPet* BotPetToControl)
{
	if (IsValid(BotPetToControl)) {
		BotPetToControl->SetControlBase(this);

		if (LootItemsClass.Num() > 0) { //override in  botPet
			int RandomIndex = FMath::RandRange(0, LootItemsClass.Num() - 1);
			BotPetToControl->OverrideLootSystem(LootItemsClass[RandomIndex], SpawnLootProbability);
		}
	}
	BotPetsControlled.Add(BotPetToControl);
	CurrentBotsCounter++;

	if (BotPetsControlled.Num() > RestSeats.Num()) {
		UE_LOG(LogTemp, Error, TEXT("BotPetControlBase has no enough RestSeats for BotPetsControlled"));
	}
}

void AUB_BotPetControlBase::UnsubscribeBotPet(AUB_BotPet* BotPetToUnsubscribe)
{
	if (IsValid(BotPetToUnsubscribe)) {
		BotPetToUnsubscribe->SetControlBase(nullptr);
	}
	BotPetsControlled.Remove(BotPetToUnsubscribe);
	CurrentBotsCounter--;

}

FVector AUB_BotPetControlBase::GetLocationRestSeat(AUB_BotPet* BotPet)
{
	int indexBotPet;
	if (BotPetsControlled.Find(BotPet, indexBotPet)) {
		if (indexBotPet < RestSeats.Num()) {
			return UKismetMathLibrary::TransformLocation(GetActorTransform(), RestSeats[indexBotPet]); //each bot has its own restSeat
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

