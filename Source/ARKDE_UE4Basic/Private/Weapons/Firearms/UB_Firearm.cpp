// Fill out your copyright notice in the Description page of Project Settings.

#include "UB_Firearm.h"
#include "UB_Character.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "..\..\..\Public\Weapons\Firearms\UB_Firearm.h"

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
	RemainingAmmo = MagazineCapacity;

	if (AvailableFireModes.Num() == 0) UE_LOG(LogTemp, Error, TEXT("No AvailableFireModes defined for this firearm"));
	SetFireMode(InitialFireModeIdx);
}

//Primary action weapon is fire
void AUB_Firearm::StartAction() {
	Fire();
}
//Secondary action weapon is reload logic
void AUB_Firearm::StartAdditionalAction() 
{
	
}

//Fire
void AUB_Firearm::Fire()
{
	GetWorldTimerManager().SetTimer(FireDelayTimer, this, &AUB_Firearm::StopFiring, FireRate, false);

	RemainingAmmo -= 1; //TODO: If this weapon shoots more than one in once, like burst, change this
}
void AUB_Firearm::StopFiring()
{
	GetWorldTimerManager().ClearTimer(FireDelayTimer);

	AUB_Character* Character = Cast<AUB_Character>(CurrentOwnerCharacter);
	if (IsValid(Character)) {
		Character->OnFinishedWeaponAction();
	}
}

//Reload
bool AUB_Firearm::IsMagazineEmpty()
{
	return RemainingAmmo <= 0;
}
bool AUB_Firearm::CanReload()
{
	return RemainingAmmo < MagazineCapacity;
}

void AUB_Firearm::Reload()
{
	RemainingAmmo = MagazineCapacity;
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
