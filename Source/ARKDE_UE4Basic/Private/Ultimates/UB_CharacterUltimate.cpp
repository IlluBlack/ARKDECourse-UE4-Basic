// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_CharacterUltimate.h"
#include "UB_Character.h"

#include "Animation/AnimMontage.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"

AUB_CharacterUltimate::AUB_CharacterUltimate()
{
	PrimaryActorTick.bCanEverTick = true;

	bTickUltimateWithTimer = true;
	UltimateDuration = 10.0f;
	UltimateFrequency = 0.5f;
	
	//Abilities
	SpeedModifier = 2.0f;
	AnimationPlayRate = 2.0f;
}

void AUB_CharacterUltimate::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(!bTickUltimateWithTimer); //at this moment because tick is just being used for ultimateBehaviour, if not remove this
}

void AUB_CharacterUltimate::SetCharacterOwner(AUB_Character* NewOwner)
{
	if (IsValid(NewOwner)) {
		SetOwner(NewOwner);
		OwnerCharacter = NewOwner;
	}
}

//Start
void AUB_CharacterUltimate::StartUltimate()
{
	if (IsValid(OwnerCharacter) && IsValid(StartAnimMontage)) {
		//Stop Movement in player
		OwnerCharacter->SetSpeedModifier(0.0f);

		//Start animation
		const float StartUltimateAnimDuration = OwnerCharacter->PlayAnimMontage(StartAnimMontage, true);
		GetWorldTimerManager().SetTimer(TimerHandle_StartUltimateBehaviour, this, &AUB_CharacterUltimate::StartUltimateBehavior, StartUltimateAnimDuration);
	}
	else {
		StartUltimateBehavior();
	}

	if (!IsValid(OwnerCharacter)) UE_LOG(LogTemp, Error, TEXT("Not OwnerCharacter assigned to this CharacterUltimate"));
	BP_StartUltimate();
}

//Start Behaviour
void AUB_CharacterUltimate::StartUltimateBehavior()
{
	bIsInUltimateBehaviour = true;

	CurrentUltimateDuration = UltimateDuration;
	if (bTickUltimateWithTimer) {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_UltimateTick, this, &AUB_CharacterUltimate::TickUltimateWithTimer, UltimateFrequency, true);
	}

	if (IsValid(OwnerCharacter)) {
		OwnerCharacter->SetSpeedModifier(SpeedModifier);
	}
}

//Tick
void AUB_CharacterUltimate::Tick(float DeltaTime) //Default actor tick
{
	Super::Tick(DeltaTime);

	if (bIsInUltimateBehaviour && !bTickUltimateWithTimer) {
		TickUltimate(DeltaTime);
	}
}
void AUB_CharacterUltimate::TickUltimate(float DeltaTime)
{
	CurrentUltimateDuration = FMath::Clamp(CurrentUltimateDuration - DeltaTime, 0.0f, UltimateDuration);
	if (CurrentUltimateDuration <= 0.0f) {
		StopUltimate();
	}
}
void AUB_CharacterUltimate::TickUltimateWithTimer()
{
	TickUltimate(UltimateFrequency);
}

//Stop
void AUB_CharacterUltimate::StopUltimate()
{
	bIsInUltimateBehaviour = false;
	if (bTickUltimateWithTimer) {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_UltimateTick);
	}

	if (IsValid(OwnerCharacter)) {
		OwnerCharacter->SetSpeedModifierToDefault();
		OwnerCharacter->OnFinishedUltimate();
	}

	BP_StopUltimate();
}
