// Fill out your copyright notice in the Description page of Project Settings.


//#include "UB_BaseButton.h"
#include "..\..\Public\UI\UB_BaseButton.h"

#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"

UUB_BaseButton::UUB_BaseButton()
{
	CheckFocusRate = 0.1f;
}

void UUB_BaseButton::Start()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CheckFocus, this, &UUB_BaseButton::CheckFocus, CheckFocusRate, true);
}

void UUB_BaseButton::CheckFocus()
{
	if (IsHovered()) { //hovered by mouse
		SetKeyboardFocus(); //set gamepad and keyboard to this
		SetButtonStyle(EUB_ButtonStyle::ButtonStyle_MouseSelected);
	}
	else if(HasKeyboardFocus()) { //check keyboard or gamepad
		SetButtonStyle(EUB_ButtonStyle::ButtonStyle_KeyboardSelected);
	}
	else {
		SetButtonStyle(EUB_ButtonStyle::ButtonStyle_Unselected);
	}
}

void UUB_BaseButton::SetButtonStyle(EUB_ButtonStyle NewStyle)
{
	BP_SetButtonStyle(NewStyle);
}
