// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/UB_Projectile.h"
#include "UB_Grenade.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_Grenade : public AUB_Projectile
{
	GENERATED_BODY()

protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	//float HitDamage; //Damage when directly hitting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	bool bTakeEnvironmentIntoAccount; //if not it does not matter if it's behind a wall, it will get damage

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	bool bHasFalloffExplosion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float ExplosionMaxDamage; //Damage in explosion center
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float ExplosionMinDamage; //Minimun damage when far from explosion center
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float ExplosionInnerRadius; //Radius of FullDamage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade", meta = (ClampMin = "0.2", UIMin = "0.2"))
	float ExplosionOuterRadius; //It's the full explosion radius

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	TSubclassOf<UDamageType> DamageType; //if there's not assigned, it takes the default one

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade|Debug")
	bool bDebugExplosion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float ExplosionDelay;
	FTimerHandle ExplosionDelayTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float DisappearDelay;
	FTimerHandle DisappearDelayTimer;

public:
	AUB_Grenade();

protected:
	UFUNCTION(BlueprintCallable, Category = "Grenade")
	void Explode();
	UFUNCTION(BlueprintCallable, Category = "Grenade")
	void Disappear();

public:
	virtual void Throw() override;

};
