// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_ANSWeaponPunch.h"
#include "UB_Character.h"
#include "UB_Weapon.h"
#include "Components/SkeletalMeshComponent.h"

void UUB_ANSWeaponPunch::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor)) {
		AUB_Character* Character = Cast<AUB_Character>(CharacterActor);
		if (IsValid(Character)) {
			AUB_Weapon* Weapon = Character->CurrentWeapon;
			if (IsValid(Weapon)) {
				Weapon->SetPunchDetectorEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
	}
}

void UUB_ANSWeaponPunch::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor)) {
		AUB_Character* Character = Cast<AUB_Character>(CharacterActor);
		if (IsValid(Character)) {
			AUB_Weapon* Weapon = Character->CurrentWeapon;
			if (IsValid(Weapon)) {
				Weapon->SetPunchDetectorEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}
