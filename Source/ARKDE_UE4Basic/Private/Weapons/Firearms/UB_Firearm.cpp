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

	AvailableFireModes = { EUB_FireMode::FireMode_SemiAutomatic };
	InitialFireModeIdx = 0;

	MaxAngleBulletSpread = 2.2f;
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
}

//Primary action firearm is FIRE
void AUB_Firearm::StartAction() 
{
	bIsWeaponActionTriggered = true;

	if (CurrentState != EUB_WeaponState::State_Idle
		&& CurrentState != EUB_WeaponState::State_AdditionalAction) return;

	//fire is more important than reload, verify first if ammo is enough to fire and stop reloading
	if (CurrentState == EUB_WeaponState::State_AdditionalAction) {
		if (IsMagazineEmpty()) return; //continue reloading, and not cancel
		CancelReloading();
	}

	if (IsMagazineEmpty()) {
		Reload();
	}
	else {
		Fire();
	}
}

//Additional action firearm is RELOAD
void AUB_Firearm::StartAdditionalAction() 
{
	if (CurrentState == EUB_WeaponState::State_Action) CancelFiring();
	Reload();
}
void AUB_Firearm::OnFinishedAdditionalAction() {
	FinishedReloading();
}

//Punch action
void AUB_Firearm::StartPunchAction()
{
	if (CurrentState == EUB_WeaponState::State_PunchAction) return;
	if (CurrentState == EUB_WeaponState::State_Action) CancelFiring();

	if (CurrentState == EUB_WeaponState::State_AdditionalAction) {
		if (IsMagazineEmpty()) return; //continue reloading, and not cancel
		CancelReloading();
	}

	Super::StartPunchAction();
}

//Fire
void AUB_Firearm::Fire()
{
	CurrentState = EUB_WeaponState::State_Action;
	BP_StartAction();

	PlayAnimMontageInOwner(FireAnimMontage);
	GetWorldTimerManager().SetTimer(FireDelayTimer, this, &AUB_Firearm::FinishedFiring, FireRate, false);

	RemainingAmmo -= 1; //TODO: If this weapon shoots more than one in once, like burst...
}
void AUB_Firearm::FinishedFiring() //is not controlled by animation, but timer
{
	CurrentState = EUB_WeaponState::State_Idle;
	GetWorldTimerManager().ClearTimer(FireDelayTimer);

	VerifyAutomaticFire();
}
void AUB_Firearm::CancelFiring()
{
	CurrentState = EUB_WeaponState::State_Idle;
	GetWorldTimerManager().ClearTimer(FireDelayTimer);

	StopAnimMontageInOwner(FireAnimMontage);
}

//AutomaticFire
void AUB_Firearm::VerifyAutomaticFire()
{
	//Verify if it's automatic weapon, and continue firing
	if (GetCurrentFireMode() == EUB_FireMode::FireMode_Automatic && 
		bIsWeaponActionTriggered) {
		StartAction();
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
	if (CurrentState == EUB_WeaponState::State_AdditionalAction) return;

	if (CanReload()) 
	{
		CurrentState = EUB_WeaponState::State_AdditionalAction;
		BP_StartAdditionalAction();

		PlayAnimMontageInOwner(ReloadAnimMontage);
	}
	else 
	{
		PlayAnimMontageInOwner(CurrentOwnerCharacter->DenyAnimMontage);
	}
}
void AUB_Firearm::FinishedReloading()
{
	CurrentState = EUB_WeaponState::State_Idle;
	FillMagazine();

	VerifyAutomaticFire();
}
void AUB_Firearm::CancelReloading()
{
	CurrentState = EUB_WeaponState::State_Idle;
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

EUB_FireMode AUB_Firearm::GetCurrentFireMode()
{
	return AvailableFireModes[CurrentFireModeIdx];
}

//Effects
void AUB_Firearm::PlayMuzzleEffect()
{
	if (IsValid(MuzzleEffect)) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMeshComponent, MuzzleSocketName);
	}
}

FVector AUB_Firearm::GetMuzzleSocketLocation() {
	return WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
}
FRotator AUB_Firearm::GetMuzzleSocketRotation() {
	return WeaponMeshComponent->GetSocketRotation(MuzzleSocketName);
}
