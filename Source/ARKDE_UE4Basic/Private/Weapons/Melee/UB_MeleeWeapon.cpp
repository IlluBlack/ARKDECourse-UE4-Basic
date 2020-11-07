// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_MeleeWeapon.h"
#include "ARKDE_UE4Basic.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AUB_MeleeWeapon::AUB_MeleeWeapon()
{
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(CustomRootComponent);
	//WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PunchDetectorComponent->SetupAttachment(WeaponMeshComponent);

	MeleeDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeDetectorComponent"));
	MeleeDetectorComponent->SetupAttachment(WeaponMeshComponent);
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetectorComponent->SetCollisionResponseToChannel(OCC_ENEMY, ECR_Overlap);
	SetMeleeDetectorEnabled(ECollisionEnabled::NoCollision);

	StepComboDamageMultiplier = 1.0f;
	CurrentDamageMultiplier = 1.0f;
}

void AUB_MeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AUB_MeleeWeapon::ApplyMeleeDamage);
}

void AUB_MeleeWeapon::StartAction()
{
	//the collider is actually enabled in the anim notify
}

void AUB_MeleeWeapon::StopAction()
{

}

//MeleeDetector
void AUB_MeleeWeapon::SetMeleeDetectorEnabled(ECollisionEnabled::Type NewCollisionState) //called from animationNotify
{
	MeleeDetectorComponent->SetCollisionEnabled(NewCollisionState);
}

void AUB_MeleeWeapon::ApplyMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor)) {
		AActor* Owner = GetOwner();

		if (IsValid(Owner)) {
			UGameplayStatics::ApplyPointDamage(OtherActor, Damage * CurrentDamageMultiplier, SweepResult.Location, SweepResult, Owner->GetInstigatorController(), this, nullptr);
		}
	}
}

//Combos
void AUB_MeleeWeapon::SetStepCombo(int Step) //this step comes from 0 to ..
{
	//When setting step combo we should change the current damage multiplier of the weapon
	CurrentDamageMultiplier = StepComboDamageMultiplier * (Step + 1);
}
