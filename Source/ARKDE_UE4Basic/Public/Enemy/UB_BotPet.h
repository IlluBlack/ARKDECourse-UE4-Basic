// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UB_BotPet.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class AUB_Character;

UCLASS()
class ARKDE_UE4BASIC_API AUB_BotPet : public APawn
{
	GENERATED_BODY()

protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//USceneComponent* CustomRootComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* BotMeshComponent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "References")
	AUB_Character* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Navigation")
	FVector NextPathPoint;
	UPROPERTY(EditDefaultsOnly, Category = "Navigation|Debug")
	bool bDrawNextPathPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MinDistanceToTarget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float ForceMagnitude;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FName ForceBoneName;

public:
	// Sets default values for this pawn's properties
	AUB_BotPet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	FVector GetNextPathPoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
