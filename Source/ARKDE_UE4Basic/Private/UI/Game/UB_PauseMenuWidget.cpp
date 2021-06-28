// Fill out your copyright notice in the Description page of Project Settings.


//#include "UB_PauseMenuWidget.h"
#include "..\..\..\Public\UI\Game\UB_PauseMenuWidget.h"
#include "Core/UB_GameInstance.h"
#include "Core/UB_GameMode.h"

#include "Kismet/GameplayStatics.h"

void UUB_PauseMenuWidget::Resume()
{
	BP_Resume();

	AUB_GameMode* GameMode = Cast<AUB_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->TogglePauseGame();
}

void UUB_PauseMenuWidget::Restart()
{
	BP_Restart();
}

void UUB_PauseMenuWidget::ReturnMainMenu()
{
	BP_ReturnMainMenu();

	//save before leaving
	UUB_GameInstance* GameInstance = Cast<UUB_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstance)) {
		GameInstance->SaveData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}
