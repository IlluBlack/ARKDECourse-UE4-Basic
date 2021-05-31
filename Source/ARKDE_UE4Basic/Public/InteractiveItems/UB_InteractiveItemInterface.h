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

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//BlueprintNativeEvent will allow adding this interface to BP
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartHover();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StopHover();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void BP_StartHover();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void BP_StopHover();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartInteraction(AActor* ActorInteracting);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StopInteraction(AActor* ActorInteracting);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void BP_StartInteraction(AActor* ActorInteracting);
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void BP_StopInteraction(AActor* ActorInteracting);
};
