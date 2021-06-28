// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UB_GameInstance.generated.h"

class UUB_SaveGame;

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API UUB_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save System")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	int DefeatedEnemies;

public:
	UUB_GameInstance();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Player Stats")
	void BP_AddDefeatedEnemies(int TotalDefeated = 1);

	UFUNCTION(BlueprintImplementableEvent, Category = "Save System")
	void BP_SaveData();
	UFUNCTION(BlueprintImplementableEvent, Category = "Save System")
	void BP_LoadData();
	UFUNCTION(BlueprintImplementableEvent, Category = "Save System")
	void BP_ResetData();

	UUB_SaveGame* GetCurrentSavedGame(bool bShouldCreateIfNull);

public:
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	int GetDefeatedEnemies() { return DefeatedEnemies; };
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void SetDefeatedEnemies(int NewValue) { DefeatedEnemies = NewValue; };

	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void AddDefeatedEnemies(int TotalDefeated = 1);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveData();
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadData();
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void ResetData();
};
