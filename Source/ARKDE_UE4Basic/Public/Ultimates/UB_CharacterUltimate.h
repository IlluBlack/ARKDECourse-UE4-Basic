// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_CharacterUltimate.generated.h"

class AUB_Character;
class UAnimMontage;
class UWorld;

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_CharacterUltimate : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	AUB_Character* OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate")
	bool bTickUltimateWithTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Duration", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float UltimateDuration;
	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Duration")
	float CurrentUltimateDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Duration")
	float UltimateFrequency;
	FTimerHandle TimerHandle_UltimateTick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	float SpeedModifier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	float AnimationPlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* StartAnimMontage;
	FTimerHandle TimerHandle_StartUltimateBehaviour;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	bool bIsInUltimateBehaviour;

public:
	AUB_CharacterUltimate();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartUltimate();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StopUltimate();

	virtual void StartUltimateBehavior();
	virtual void TickUltimate(float DeltaTime);
	void TickUltimateWithTimer();
	virtual void StopUltimate();

public:
	virtual void Tick(float DeltaTime) override;

	void SetCharacterOwner(AUB_Character* NewOwner);
	void StartUltimate();
};
