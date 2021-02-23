// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItems/UB_CollectableItem.h"
#include "UB_HealthItem.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_HealthItem : public AUB_CollectableItem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HealthItemMeshComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthItem")
	float HealthValue;

public:
	AUB_HealthItem();

protected:
	virtual void Pickup(AUB_Character* PickupCharacter) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetHealthValue() const { return HealthValue; };

};
