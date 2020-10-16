// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/UB_Weapon.h"
#include "UB_Rifle.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_Rifle : public AUB_Weapon
{
	GENERATED_BODY()
	
protected:
	virtual void StartAction() override;
	virtual void StopAction() override;

};
