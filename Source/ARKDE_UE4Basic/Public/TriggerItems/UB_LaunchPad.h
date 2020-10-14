// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UB_TriggerItem.h"
#include "UB_LaunchPad.generated.h"

class UMaterial;

UCLASS()
class ARKDE_UE4BASIC_API AUB_LaunchPad : public AUB_TriggerItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LauchPadMeshComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LauchPad")
	FVector LaunchVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LauchPad")
	bool bXYOverride;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LauchPad")
	bool bZOverride; //if wants to override character is always gonna take that velocity instead of add to its movement

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsActive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	UMaterial* ActiveMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	UMaterial* DisableMaterial;

	
public:	
	// Sets default values for this actor's properties
	AUB_LaunchPad();

	void SetActive(bool bSetActive);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnCharacterOverlap(AUB_Character* Character) override;
	void ResetMaterial();
};
