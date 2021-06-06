// Fill out your copyright notice in the Description page of Project Settings.


//#include "UB_MainMenuWidget.h"
#include "..\..\..\Public\UI\MainMenu\UB_MainMenuWidget.h"

#include "UI/UB_BaseButton.h"

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
}

void UUB_MainMenuWidget::ContinueGame()
{
	BP_ContinueGame();
}

void UUB_MainMenuWidget::QuitGame()
{
	BP_QuitGame();
}
