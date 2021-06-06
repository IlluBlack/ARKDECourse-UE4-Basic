// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UB_BaseButton.generated.h"

UENUM(Blueprintable)
enum class EUB_ButtonStyle : uint8
{
	ButtonStyle_MouseSelected		UMETA(displayName = "Selected by mouse"),
	ButtonStyle_KeyboardSelected	UMETA(DisplayName = "Selected by keyboard"), //this includes gamepad
	ButtonStyle_Unselected			UMETA(DisplayName = "Unselected"),
};

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API UUB_BaseButton : public UButton
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base button")
	float CheckFocusRate;

	FTimerHandle TimerHandle_CheckFocus;

public:
	UUB_BaseButton();

protected:
	void CheckFocus();
	void SetButtonStyle(EUB_ButtonStyle NewStyle);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base button")
	void BP_SetButtonStyle(EUB_ButtonStyle NewStyle);

public:
	void Start();
};
