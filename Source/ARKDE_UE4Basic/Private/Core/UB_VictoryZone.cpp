// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_VictoryZone.h"
#include "Core/UB_GameMode.h"
#include "UB_Character.h"
#include "Engine/World.h"

void AUB_VictoryZone::BeginPlay()
{
	Super::BeginPlay();

	GameModeReference = Cast<AUB_GameMode>(GetWorld()->GetAuthGameMode());
}

void AUB_VictoryZone::OnCharacterOverlap(AUB_Character* Character)
{
	if (Character->GetCharacterType() == EUB_CharacterType::CharacterType_Player) {
		if (IsValid(GameModeReference)) {
			Super::OnCharacterOverlap(Character);
			GameModeReference->Victory(Character);
		}
	}
}
