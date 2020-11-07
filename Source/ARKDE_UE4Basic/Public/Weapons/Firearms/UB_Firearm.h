// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/UB_Weapon.h"
#include "UB_Firearm.generated.h"

class UParticleSystem;

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
	float FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firearm")
	float MagazineCapacity; //number of rounds bullets before having to reload

	FTimerHandle FireDelayTimer; //use FireRate

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firearm")
	int RemainingAmmo; //how many bullets do I have left

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzzle")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* MuzzleEffect; //all fire arms should have a muzzle effect

public:
	AUB_Firearm();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Firearm")
	virtual void Fire();
	void StopFiring();

	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	void PlayMuzzleEffect();
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FVector GetMuzzleSocketLocation();
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FRotator GetMuzzleSocketRotation();

	void SetFireMode(int IdxMode);

public:
	virtual void StartAction() override;
	virtual void StartAdditionalAction() override;

	bool IsMagazineEmpty();
	bool CanReload();
	UFUNCTION(BlueprintCallable, Category = "Firearm")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category = "Firearm") //If the weapon has another mode change it
	virtual void ChangeWeaponMode();

	EFireMode GetCurrentFireMode();

};
