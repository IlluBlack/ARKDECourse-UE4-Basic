// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_GameInstance.h"
#include "SaveSystem/UB_SaveGame.h"

#include "Kismet/GameplayStatics.h"

UUB_GameInstance::UUB_GameInstance()
{
	SaveSlotName = "UB_SaveData";
}

void UUB_GameInstance::AddDefeatedEnemies(int TotalDefeated)
{
	DefeatedEnemies += TotalDefeated;
	BP_AddDefeatedEnemies(TotalDefeated);
}

UUB_SaveGame* UUB_GameInstance::GetCurrentSavedGame(bool bShouldCreateIfNull)
{
	USaveGame* SavedGame = nullptr;

	bool bSavedData = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);
	if (bSavedData) {
		SavedGame = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	}
	else if(bShouldCreateIfNull){
		SavedGame = UGameplayStatics::CreateSaveGameObject(UUB_SaveGame::StaticClass());
	}

	if (IsValid(SavedGame)) {
		return Cast<UUB_SaveGame>(SavedGame);
	}

	return nullptr;
}

void UUB_GameInstance::SaveData()
{
	UUB_SaveGame* SaveFile = GetCurrentSavedGame(true);
	if (IsValid(SaveFile)) {
		SaveFile->SetDefeatedEnemies(DefeatedEnemies);
		//...

		UGameplayStatics::SaveGameToSlot(SaveFile, SaveSlotName, 0);
	}
}

void UUB_GameInstance::LoadData()
{
	UUB_SaveGame* SaveFile = GetCurrentSavedGame(false);
	if (IsValid(SaveFile)) {
		DefeatedEnemies = SaveFile->GetDefeatedEnemies();
		//...
	}
}

void UUB_GameInstance::ResetData()
{
	DefeatedEnemies = 0;

	BP_ResetData();
}
