// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UB_MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API UUB_MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main Menu")
	FName GameplayMapName;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Main Menu") //call from construct function
	void StartButtons(TArray<UUB_BaseButton*> NavigableButtons, UUB_BaseButton* InitialFocused);

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void NewGame();
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void ContinueGame();
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void QuitGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu")
	void BP_NewGame();
	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu")
	void BP_ContinueGame();
	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu")
	void BP_QuitGame();

};
