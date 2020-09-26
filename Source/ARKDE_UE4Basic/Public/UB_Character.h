// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UB_Character.generated.h"

UCLASS()
class ARKDE_UE4BASIC_API AUB_Character : public ACharacter
{
	GENERATED_BODY()

//Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		bool bIsInvertedLook;

//Functions
public:
	// Sets default values for this character's properties
	AUB_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	virtual void Jump() override;
	virtual void StopJumping() override;

	virtual void AddControllerPitchInput(float value) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
