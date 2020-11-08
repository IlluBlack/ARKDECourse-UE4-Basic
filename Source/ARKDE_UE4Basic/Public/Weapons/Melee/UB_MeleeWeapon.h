// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/UB_Weapon.h"
#include "UB_MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_MeleeWeapon : public AUB_Weapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* MeleeDetectorComponent; //This collider can be different from the punch detector, I made another one because it can depend on the type of melee weapon

protected:
	//Melee weapons can have combos or not, if meleeAnimMontages is > 1 then there is combos
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firearm")
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combos")
	bool bIsMeleeComboEnabled; //combos are enabled just in some specific part of the animation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combos")
	int CurrentStepMeleeCombo; //starts in zero

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combos", meta = (EditCondition = bHasCombos, ClampMin = "1.0", UIMin = "1.0"))
	float StepComboDamageMultiplier; //how much does it add in each combo
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combos")
	float CurrentDamageMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<UAnimMontage*> MeleeAnimMontages; //melee animation or combo animations

public:
	AUB_MeleeWeapon();

protected:
	virtual void BeginPlay() override;

	//Melee detector
	UFUNCTION()
	virtual void ApplyMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void EquipWeapon() override;

	virtual void StartAction() override;
	virtual void StartAdditionalAction() override;
	
	//Anim notifier
	virtual void OnFinishedAction() override;

	//Melee detector
	void EnableMeleeDetector(); //EnableCollider
	void DisableMeleeDetector(); //EnableCollider

	//Combos : public because can be called from animations
	void ResetMeleeCombo();
	void SetMeleeComboEnabled(bool NewState);
};
