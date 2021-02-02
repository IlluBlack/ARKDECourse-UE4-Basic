// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/UB_Weapon.h"
#include "UB_Firearm.generated.h"

class UParticleSystem;
class UAnimMontage;

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SemiAutomatic,
	Automatic,
	//Burst //TODO: Maybe add this later
};


UCLASS()
class ARKDE_UE4BASIC_API AUB_Firearm : public AUB_Weapon
{
	GENERATED_BODY()

protected:
	//All weapons in this projects are skeletalMesh, then I am not using UStaticMesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMeshComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firearm")
	TArray<EFireMode> AvailableFireModes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firearm")
	int InitialFireModeIdx;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firearm")
	int CurrentFireModeIdx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firearm")
	float MaxAngleBulletSpread;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firearm")
	float FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firearm")
	float MagazineCapacity; //number of rounds bullets before having to reload

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firearm")
	bool bIsFiring;
	FTimerHandle FireDelayTimer; //use FireRate

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firearm")
	int RemainingAmmo; //how many bullets do I have left
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firearm")
	bool bIsReloading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzzle")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* MuzzleEffect; //all fire arms should have a muzzle effect

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* ReloadAnimMontage; //we put this in this class and not the character because reload animation can differ between firearms

public:
	AUB_Firearm();

protected:
	virtual void BeginPlay() override;

	//Fire
	UFUNCTION(BlueprintCallable, Category = "Firearm")
	virtual void Fire();
	void FinishedFiring();
	void CancelFiring();

	//Automatic fire
	virtual void VerifyAutomaticFire();

	//Reload
	void FillMagazine();
	bool IsMagazineEmpty() const;
	bool CanReload() const;
	UFUNCTION(BlueprintCallable, Category = "Firearm")
	virtual void Reload();
	virtual void FinishedReloading();
	void CancelReloading();

	//Modes
	void SetFireMode(int IdxMode);
	virtual void ChangeWeaponMode() override;

	//Effects
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	void PlayMuzzleEffect();
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FVector GetMuzzleSocketLocation();
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FRotator GetMuzzleSocketRotation();

public:
	virtual void StartAction() override;
	virtual void StartAdditionalAction() override;
	virtual void StartPunchAction() override;

	//Anim notifier
	virtual void OnFinishedAdditionalAction() override;

	EFireMode GetCurrentFireMode();
};
