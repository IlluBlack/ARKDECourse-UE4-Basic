// Fill out your copyright notice in the Description page of Project Settings.

#include "UB_Firearm.h"
#include "UB_Character.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Components/CapsuleComponent.h"

AUB_Firearm::AUB_Firearm()
{
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(CustomRootComponent);
	//WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PunchDetectorComponent->SetupAttachment(WeaponMeshComponent);

	AvailableFireModes = { EFireMode::SemiAutomatic };
	InitialFireModeIdx = 0;

	FireRate = 1.0f;
	MagazineCapacity = 8;

	MuzzleSocketName = "SCK_Muzzle";
}

void AUB_Firearm::BeginPlay()
{
	Super::BeginPlay();

	if (MagazineCapacity <= 0) UE_LOG(LogTemp, Error, TEXT("MagazineCapacity for this firearm is zero or less"));
	FillMagazine();

	if (AvailableFireModes.Num() == 0) UE_LOG(LogTemp, Error, TEXT("No AvailableFireModes defined for this firearm"));
	SetFireMode(InitialFireModeIdx);

	bIsReloading = false;
}

//Primary action weapon is FIRE
void AUB_Firearm::StartAction() 
{
	Super::StartAction();
	if (bIsPunching) return; //TODO: Remove later, make work return work from base

	//fire is more important than reload, verify first if ammo is enough to fire and stop reloading
	if (bIsReloading) {
		if (IsMagazineEmpty()) return; //continue reloading, and not cancel
		CancelReloading();
	}

	if (bIsFiring) {
		return;
	}

	if (IsMagazineEmpty()) {
		Reload();
	}
	else {
		Fire();
	}
}

//Additional action weapon is RELOAD
void AUB_Firearm::StartAdditionalAction() 
{
	Super::StartAdditionalAction();
	if (bIsPunching) return; //TODO: Remove later, make work return work from base

	if (bIsFiring) CancelFiring();
	Reload();
}
void AUB_Firearm::OnFinishedAdditionalAction()
{
	Super::OnFinishedAdditionalAction();
	FinishedReloading();
}

//Punch action override because we need to verify is not reloading, or shooting
void AUB_Firearm::StartPunchAction()
{
	if (bIsPunching) return;

	//if (bIsFiring) return;
	if (bIsFiring) CancelFiring();

	if (bIsReloading) {
		if (IsMagazineEmpty()) return; //continue reloading, and not cancel
		CancelReloading();
	}

	//now, yeah, punch
	Super::StartPunchAction();
}

//Fire
void AUB_Firearm::Fire()
{
	bIsFiring = true;
	GetWorldTimerManager().SetTimer(FireDelayTimer, this, &AUB_Firearm::FinishedFiring, FireRate, false);

	RemainingAmmo -= 1; //TODO: If this weapon shoots more than one in once, like burst, change this
}
void AUB_Firearm::FinishedFiring() //is not controlled by animation, but timer
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(FireDelayTimer);

	VerifyAutomaticFire();
}
void AUB_Firearm::CancelFiring()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(FireDelayTimer); //clear timer for it to not be called
}

//AutomaticFire
void AUB_Firearm::VerifyAutomaticFire() //Verify if it's automatic weapon, and continue firing
{
	//Verify if it's automatic weapon, and continue firing
	if (GetCurrentFireMode() == EFireMode::Automatic) {
		if (IsValid(CurrentOwnerCharacter)) {
			if (CurrentOwnerCharacter->bIsPressingWeaponAction) {
				StartAction();
			}
		}
	}
}

//Reload
bool AUB_Firearm::IsMagazineEmpty() const
{
	return RemainingAmmo <= 0;
}
bool AUB_Firearm::CanReload() const
{
	return RemainingAmmo < MagazineCapacity;
}
void AUB_Firearm::FillMagazine()
{
	RemainingAmmo = MagazineCapacity;
}
void AUB_Firearm::Reload()
{
	if (bIsReloading) return;

	if (CanReload()) {
		bIsReloading = true;
		//Call animation in owner
		PlayAnimMontageInOwner(ReloadAnimMontage);
	}
	else {
		PlayAnimMontageInOwner(CurrentOwnerCharacter->DenyAnimMontage);
	}
}
void AUB_Firearm::FinishedReloading()
{
	bIsReloading = false;
	FillMagazine();

	VerifyAutomaticFire();
}
void AUB_Firearm::CancelReloading()
{
	bIsReloading = false;
	//Stop animation in owner
	StopAnimMontageInOwner(ReloadAnimMontage);
}

//FireModes
void AUB_Firearm::SetFireMode(int IdxMode)
{
	CurrentFireModeIdx = IdxMode;
}
void AUB_Firearm::ChangeWeaponMode() //change to the next one
{
	Super::ChangeWeaponMode();

	int nextIdx = CurrentFireModeIdx + 1;
	if (nextIdx >= AvailableFireModes.Num()) {
		nextIdx = 0; //Reset
	}

	SetFireMode(nextIdx);
}

EFireMode AUB_Firearm::GetCurrentFireMode()
{
	return AvailableFireModes[CurrentFireModeIdx];
}

//Effects
void AUB_Firearm::PlayMuzzleEffect()
{
	if (IsValid(MuzzleEffect)) {
		//AUB_Character* newCast = Cast<AUB_Character> Owner;
		//UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, CurrentOwnerCharacter->GetMesh(), MuzzleSocketName);

		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMeshComponent, MuzzleSocketName);
	}
}

FVector AUB_Firearm::GetMuzzleSocketLocation()
{
	return WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
}
FRotator AUB_Firearm::GetMuzzleSocketRotation()
{
	return WeaponMeshComponent->GetSocketRotation(MuzzleSocketName);
}
