// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_ANSWeaponMelee.h"
#include "UB_Character.h"
#include "UB_MeleeWeapon.h"
#include "Components/SkeletalMeshComponent.h"

void UUB_ANSWeaponMelee::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor)) {
		AUB_Character* Character = Cast<AUB_Character>(CharacterActor);
		if (IsValid(Character)) {
			AUB_MeleeWeapon* MeleeWeapon = Cast<AUB_MeleeWeapon>(Character->CurrentWeapon);
			if (IsValid(MeleeWeapon)) {
				MeleeWeapon->EnableMeleeDetector();
			}
		}
	}
}

void UUB_ANSWeaponMelee::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor)) {
		AUB_Character* Character = Cast<AUB_Character>(CharacterActor);
		if (IsValid(Character)) {
			AUB_MeleeWeapon* MeleeWeapon = Cast<AUB_MeleeWeapon>(Character->CurrentWeapon);
			if (IsValid(MeleeWeapon)) {
				MeleeWeapon->DisableMeleeDetector();
			}
		}
	}
}
