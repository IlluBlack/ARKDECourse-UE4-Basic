// Fill out your copyright notice in the Description page of Project Settings.

#include "UB_Firearm.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/Engine/Public/TimerManager.h"

AUB_Firearm::AUB_Firearm()
{
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(RootComponent);
	//WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AvailableFireModes = { EFireMode::SemiAutomatic };
	InitialFireModeIdx = 0;

	FireRate = 1.0f;
	MagazineCapacity = 8;
	DurationReload = 1.0f;

	MuzzleSocketName = "SCK_Muzzle";
}

void AUB_Firearm::BeginPlay()
{
	Super::BeginPlay();

	if (MagazineCapacity <= 0) UE_LOG(LogTemp, Error, TEXT("MagazineCapacity for this firearm is zero or less"));
	bWantsToFire = false;
	bCanFire = true;
	RemainingAmmo = MagazineCapacity;

	if (AvailableFireModes.Num() == 0) UE_LOG(LogTemp, Error, TEXT("No AvailableFireModes defined for this firearm"));
	SetFireMode(InitialFireModeIdx);
}

//Action weapon
void AUB_Firearm::StartAction()
{
	bWantsToFire = true;
	if (bCanFire) {
		if (RemainingAmmo > 0) {
			Fire();
		}
		else {
			Reload();
		}
	}
}
void AUB_Firearm::StopAction()
{
	bWantsToFire = false;
}

//When automatic firearm
void AUB_Firearm::VerifyAutomaticFire()
{
	//Continue firing if it's still holding
	if (AvailableFireModes[CurrentFireModeIdx] == EFireMode::Automatic) {
		if (bWantsToFire && bCanFire) {
			if (RemainingAmmo > 0) {
				Fire();
			}
			else {
				Reload();
			}
		}
	}
}

//Fire
void AUB_Firearm::Fire()
{
	bCanFire = false;
	GetWorldTimerManager().SetTimer(FireDelayTimer, this, &AUB_Firearm::StopFiring, FireRate, false);

	RemainingAmmo -= 1; //TODO: If this weapon shoots more than one in once, like burst, change this
}
void AUB_Firearm::StopFiring()
{
	bCanFire = true;
	GetWorldTimerManager().ClearTimer(FireDelayTimer);

	VerifyAutomaticFire();
}

//Reload
void AUB_Firearm::Reload()
{
	if (RemainingAmmo < MagazineCapacity) {
		bCanFire = false;
		//TODO: This should play an animation
		//I added a timer while is reloading
		GetWorldTimerManager().SetTimer(ReloadDelayTimer, this, &AUB_Firearm::StopReloading, DurationReload, false);
	}
	else {
		//TODO: Play animation like no I can't reload, or sound...
	}
}
void AUB_Firearm::StopReloading()
{
	RemainingAmmo = MagazineCapacity;
	GetWorldTimerManager().ClearTimer(ReloadDelayTimer);

	bCanFire = true;
	VerifyAutomaticFire();
}

//FireModes
void AUB_Firearm::SetFireMode(int IdxMode)
{
	CurrentFireModeIdx = IdxMode;
}
void AUB_Firearm::ChangeWeaponMode() //change to the next one
{
	int nextIdx = CurrentFireModeIdx + 1;
	if (nextIdx >= AvailableFireModes.Num()) {
		nextIdx = 0; //Reset
	}

	SetFireMode(nextIdx);
}

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
