// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_ANWeaponPunchEnd.h"
#include "UB_Character.h"
#include "Components/SkeletalMeshComponent.h"

void UUB_ANWeaponPunchEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor)) {
		AUB_Character* Character = Cast<AUB_Character>(CharacterActor);
		if (IsValid(Character)) {
			Character->OnFinishedWeaponPunchAction(); //notify it's not longer in that animation
		}
	}
}
