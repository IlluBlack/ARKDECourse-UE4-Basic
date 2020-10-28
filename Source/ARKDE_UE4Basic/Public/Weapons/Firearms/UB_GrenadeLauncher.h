// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Firearms/UB_Firearm.h"
#include "UB_GrenadeLauncher.generated.h"

class AUB_Projectile;

UCLASS()
class ARKDE_UE4BASIC_API AUB_GrenadeLauncher : public AUB_Firearm
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade Launcher")
	TSubclassOf<AUB_Projectile> ProjectileClass;

public:
	AUB_GrenadeLauncher();

protected:
	virtual void Fire() override;
	
};
