// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/UB_Projectile.h"
#include "UB_ThrowableItem.generated.h"

class UPrimitiveComponent;

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_ThrowableItem : public AUB_Projectile
{
	GENERATED_BODY()

protected:
	bool bNotifiedNoise;
	
protected:
	AUB_ThrowableItem();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void NotifyAINoiseEvent();

public:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

};
