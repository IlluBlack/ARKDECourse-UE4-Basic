// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_BotPetControlBase.generated.h"

class AUB_Character;
class AUB_BotPet;
class UBillboardComponent;

UCLASS()
class ARKDE_UE4BASIC_API AUB_BotPetControlBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* SpawnerBillboardComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	bool bIsActive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 1, ClampMin = 1))
	int MaxBotsCounter;
	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	int CurrentBotsCounter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<AUB_BotPet> BotPetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> SpawnPoints;

	FTimerHandle TimerHandle_SpawnBot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Control Base")
	float ControlRadius;
	TArray<AUB_BotPet*> BotPetsControlled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RestSeats", meta = (MakeEditWidget = true))
	TArray<FVector> RestSeats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Control Base|Debug")
	bool bDebug;
	
public:	
	// Sets default values for this actor's properties
	AUB_BotPetControlBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnBot();
	FVector GetSpawnPoint();

public:
	void SuscribeBotPet(AUB_BotPet* BotPetToControl);
	void UnsubscribeBotPet(AUB_BotPet* BotPetToUnsubscribe);
	FVector GetLocationRestSeat(AUB_BotPet* BotPet);

	TArray<AUB_Character*> GetCharactersInControlRadius();

	UFUNCTION(BlueprintCallable)
	float GetControlRadius() const { return ControlRadius; };

};
