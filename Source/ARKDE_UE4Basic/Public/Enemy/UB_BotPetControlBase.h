// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_BotPetControlBase.generated.h"

class AUB_Character;
class AUB_BotPet;

UCLASS()
class ARKDE_UE4BASIC_API AUB_BotPetControlBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Control Base")
	float ControlRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Contol Base", meta = (MakeEditWidget = true))
	TArray<FVector> RestSeats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Control Base|Debug")
	bool bDebug;

	TArray<AUB_BotPet*> BotPetsControlled;
	
public:	
	// Sets default values for this actor's properties
	AUB_BotPetControlBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SuscribeBotPet(AUB_BotPet* BotPetToControl);
	FVector GetLocationRestSeat(AUB_BotPet* BotPet);

	TArray<AUB_Character*> GetCharactersInControlRadius();

	UFUNCTION(BlueprintCallable)
	float GetControlRadius() const { return ControlRadius; };

};
