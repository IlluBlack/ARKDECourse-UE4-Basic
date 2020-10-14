// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItems/UB_CollectableItem.h"
#include "UB_DoorKey.generated.h"

class UStaticMeshComponent;

UCLASS()
class ARKDE_UE4BASIC_API AUB_DoorKey : public AUB_CollectableItem
{
	GENERATED_BODY()

//Components
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* KeyMeshComponent;

//Variables
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	FName KeyTag; //tag to know what doors can this key open
	
//Functions
public:
	// Constructor
	AUB_DoorKey();

protected:
	virtual void Pickup(AUB_Character* PickupCharacter) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Getter")
	FName GetKeyTag() const { return KeyTag; };
};
