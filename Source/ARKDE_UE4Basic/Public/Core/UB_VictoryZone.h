// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerItems/UB_TriggerItem.h"
#include "UB_VictoryZone.generated.h"

class AUB_GameMode;
/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_VictoryZone : public AUB_TriggerItem
{
	GENERATED_BODY()

protected:
	AUB_GameMode* GameModeReference;

protected:
	virtual void BeginPlay() override;
	virtual void OnCharacterOverlap(AUB_Character* Character) override;

};
