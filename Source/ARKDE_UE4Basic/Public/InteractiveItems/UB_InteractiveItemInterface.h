// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UB_InteractiveItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUB_InteractiveItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARKDE_UE4BASIC_API IUB_InteractiveItemInterface
{
	GENERATED_BODY()

protected:
	bool bIsInteracting;
	AActor* InteractingWith;
	FTimerHandle TimerHandle_HoldInteraction;
	FTimerDelegate TimerDelegate_HoldInteraction;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartInteraction(AActor* ActorInteracting);
	virtual void StopInteraction(AActor* ActorInteracting);


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
	void BP_StartInteraction(AActor* ActorInteracting);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
	void BP_StopInteraction(AActor* ActorInteracting);
};
