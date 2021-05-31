// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveItems/UB_ActivateBotsPlatform.h"

#include "Enemy/UB_BotPetControlBase.h"
#include "UB_Character.h"
#include "UB_CharacterInventory.h"

// Sets default values
AUB_ActivateBotsPlatform::AUB_ActivateBotsPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeyTagToDeactivate = "KeyBotsPlatform";
	HoldTimeToFireInteraction = 5.0f;
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

bool AUB_ActivateBotsPlatform::CanStartInteraction(AActor* ActorInteracting)
{
	if (!Super::CanStartInteraction(ActorInteracting)) return false;

	//Verify if actor is a character
	if (IsValid(ActorInteracting)) {
		AUB_Character* CharacterInteracting = Cast<AUB_Character>(ActorInteracting);
		if (IsValid(CharacterInteracting)) {
			return true;
		}
	}

	return false;
}

void AUB_ActivateBotsPlatform::FireInteraction(AActor* ActorInteracting)
{
	AUB_Character* CharacterInteracting = Cast<AUB_Character>(ActorInteracting);
	if (IsValid(CharacterInteracting)) 
	{
		if (bIsPlatformActive) 
		{ //deactive if has key
			UUB_CharacterInventory* CharacterInventory = CharacterInteracting->GetInventory();
			if (IsValid(CharacterInventory)) {
				if (CharacterInventory->TryUseKey(KeyTagToDeactivate)) {
					DeactivatePlatform();
				}
			}
		}
		else {
			ActivatePlatform();
		}
	}

	Super::FireInteraction(ActorInteracting);
}

