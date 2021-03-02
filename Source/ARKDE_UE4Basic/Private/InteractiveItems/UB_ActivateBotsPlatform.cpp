// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveItems/UB_ActivateBotsPlatform.h"
//#include "..\..\Public\InteractiveItems\UB_ActivateBotsPlatform.h"

#include "InteractiveItems/UB_InteractiveItemInterface.h"
#include "Enemy/UB_BotPetControlBase.h"
#include "UB_Character.h"
#include "UB_CharacterInventory.h"

#include "Engine/Classes/Engine/World.h"
#include "Engine/Public/TimerManager.h"

// Sets default values
AUB_ActivateBotsPlatform::AUB_ActivateBotsPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeyTagToDeactivate = "KeyBotsPlatform";
	HoldTimeToDeactivate = 5.0f;
}

// Called when the game starts or when spawned
void AUB_ActivateBotsPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	DeactivatePlatform(true);
}

// Called every frame
void AUB_ActivateBotsPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUB_ActivateBotsPlatform::ActivatePlatform()
{
	if (bIsPlatformActive) return;

	for (AUB_BotPetControlBase* ControlBase : BotControlBases) {
		if (IsValid(ControlBase)) {
			ControlBase->ActivateSpawner();
		}
	}
	bIsPlatformActive = true;

	BP_ActivatePlatform();
}

void AUB_ActivateBotsPlatform::DeactivatePlatform(bool bForceDeactivation)
{
	if (!bIsPlatformActive && !bForceDeactivation) return;

	for (AUB_BotPetControlBase* ControlBase : BotControlBases) {
		if (IsValid(ControlBase)) {
			ControlBase->DeactivateSpawner();
		}
	}
	bIsPlatformActive = false;

	BP_DeactivatePlatform();
}

void AUB_ActivateBotsPlatform::StartInteraction(AActor* ActorInteracting)
{
	IUB_InteractiveItemInterface::Execute_BP_StartInteraction(this, ActorInteracting);

	if (bIsPlatformActive && !bIsInteracting) {
		//Verify if player has key
		if (IsValid(ActorInteracting)) {
			AUB_Character* CharacterInteracting = Cast<AUB_Character>(ActorInteracting);
			if (IsValid(CharacterInteracting)) {

				UUB_CharacterInventory* CharacterInventory = CharacterInteracting->GetInventory();
				if (IsValid(CharacterInventory)) {
					if (CharacterInventory->HasKey(KeyTagToDeactivate)) {
						//Start timer if player holds
						bIsInteracting = true;
						TimerDelegate_HoldInteraction = FTimerDelegate::CreateUObject(this, &AUB_ActivateBotsPlatform::EndHoldInteraction, CharacterInteracting);
						//TimerDelegate_HoldInteraction.AddDynamic(this, &AUB_ActivateBotsPlatform::EndHoldInteraction(CharacterInteracting));
						GetWorldTimerManager().SetTimer(TimerHandle_HoldInteraction, TimerDelegate_HoldInteraction, HoldTimeToDeactivate, false);
					}
					else {
						//TODO Play error sound
					}
				}
			}
		}
	}
	else {
		ActivatePlatform();
	}
}

void AUB_ActivateBotsPlatform::StopInteraction(AActor* ActorInteracting)
{
	IUB_InteractiveItemInterface::Execute_BP_StopInteraction(this, ActorInteracting);

	if (bIsInteracting && InteractingWith == ActorInteracting) {
		GetWorldTimerManager().ClearTimer(TimerHandle_HoldInteraction);
		bIsInteracting = false;
		InteractingWith = nullptr;
	}
}

void AUB_ActivateBotsPlatform::EndHoldInteraction(AUB_Character* CharacterInteracting) {
	//TimerDelegate_HoldInteraction.Unbind();

	if (IsValid(CharacterInteracting)) {
		UUB_CharacterInventory* CharacterInventory = CharacterInteracting->GetInventory();
		if (IsValid(CharacterInventory)) {
			if (CharacterInventory->TryUseKey(KeyTagToDeactivate)) {
				DeactivatePlatform();
			}
		}
	}
}

