// Fill out your copyright notice in the Description page of Project Settings.


//#include "UB_MainMenuWidget.h"
#include "..\..\..\Public\UI\MainMenu\UB_MainMenuWidget.h"
#include "Core/UB_GameInstance.h"

#include "UI/UB_BaseButton.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Kismet/KismetSystemLibrary.h"

void UUB_MainMenuWidget::StartButtons(TArray<UUB_BaseButton*> NavigableButtons, UUB_BaseButton* InitialFocused)
{
	for (UUB_BaseButton* Button : NavigableButtons) {
		Button->Start();
	}

	InitialFocused->SetKeyboardFocus();
}

void UUB_MainMenuWidget::NewGame()
{
	BP_NewGame();

	UUB_GameInstance* GameInstance = Cast<UUB_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstance)) {
		GameInstance->ResetData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), GameplayMapName);
}

void UUB_MainMenuWidget::ContinueGame()
{
	BP_ContinueGame();

	UUB_GameInstance* GameInstance = Cast<UUB_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstance)) {
		GameInstance->LoadData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), GameplayMapName);
}

void UUB_MainMenuWidget::QuitGame()
{
	BP_QuitGame();

	UObject* World = GetWorld();
	APlayerController* Controller = UGameplayStatics::GetPlayerController(World, 0);
	UKismetSystemLibrary::QuitGame(World, Controller, EQuitPreference::Quit, false);
}
