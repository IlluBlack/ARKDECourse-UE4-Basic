// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/UB_Weapon.h"
#include "UB_Rifle.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_Rifle : public AUB_Weapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
	float TraceLenght;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace|Debug")
	bool bDebugLineTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName TraceEndParamName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* TraceEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ImpactEffect;

public:
	AUB_Rifle();
	
protected:
	virtual void StartAction() override;
	virtual void StopAction() override;

};
