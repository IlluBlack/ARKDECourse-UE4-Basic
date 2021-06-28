// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UB_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API UUB_SaveGame : public USaveGame
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Data/Player Stats")
	int DefeatedEnemies;

public:
	UFUNCTION(BlueprintCallable, Category = "Data/Player Stats")
	int GetDefeatedEnemies() { return DefeatedEnemies; };
	UFUNCTION(BlueprintCallable, Category = "Data/Player Stats")
	void SetDefeatedEnemies(int NewValue) { DefeatedEnemies = NewValue; };
	
};
