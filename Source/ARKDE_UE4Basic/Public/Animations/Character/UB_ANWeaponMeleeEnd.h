// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UB_ANWeaponMeleeEnd.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API UUB_ANWeaponMeleeEnd : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
