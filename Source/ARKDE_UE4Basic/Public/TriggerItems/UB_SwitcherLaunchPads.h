// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UB_TriggerItem.h"
#include "UB_SwitcherLaunchPads.generated.h"

class AUB_LaunchPad;

UCLASS()
class ARKDE_UE4BASIC_API AUB_SwitcherLaunchPads : public AUB_TriggerItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SwitcherMeshComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switcher")
	TArray<AUB_LaunchPad*> ControlLaunchPads;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switcher")
	float DurationLaunchPadsActivation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Switcher")
	bool bIsOn;
	UPROPERTY(BlueprintReadOnly, Category = "Switcher")
	float CurrentActiveTime;

public:
	// Sets default values for this actor's properties
	AUB_SwitcherLaunchPads();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnCharacterOverlap(AUB_Character* Character) override;
	
	void ResetLaunchPadsState();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
