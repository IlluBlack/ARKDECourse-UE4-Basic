// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/UB_Projectile.h"
#include "UB_Grenade.generated.h"

class UUB_ExplosionComponent;

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_Grenade : public AUB_Projectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUB_ExplosionComponent* ExplosionComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float DisappearDelay;
	FTimerHandle DisappearDelayTimer;

public:
	AUB_Grenade();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Grenade")
	void Disappear();

	UFUNCTION()
	void OnExplode(UUB_ExplosionComponent* CurrentExplosionComponent, const TArray<AActor*> OverlappedActors);

public:
	virtual void Throw() override;

};
