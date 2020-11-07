// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_ANWeaponMeleeEnd.h"
#include "UB_Character.h"
#include "Components/SkeletalMeshComponent.h"

void UUB_ANWeaponMeleeEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor)) {
		AUB_Character* Character = Cast<AUB_Character>(CharacterActor);
		if (IsValid(Character)) {
			Character->OnFinishedWeaponAction();
		}
	}
}
