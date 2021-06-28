// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UB_PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API UUB_PauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pause Menu")
	FName MainMenuMapName;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void Resume();
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void Restart();
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void ReturnMainMenu();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_Resume();
	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_Restart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_ReturnMainMenu();
};
