// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_MeleeWeapon.h"
#include "UB_Character.h"
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
	DisableMeleeDetector();

	StepComboDamageMultiplier = 1.0f;
	CurrentDamageMultiplier = 1.0f;
}

void AUB_MeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AUB_MeleeWeapon::ApplyMeleeDamage);
}

void AUB_MeleeWeapon::EquipWeapon()
{
	Super::EquipWeapon();

	ResetMeleeCombo();
}

//Primary Action is attack with the melee weapon
void AUB_MeleeWeapon::StartAction()
{
	Super::StartAction();

	//the melee collider is enabled in the anim notify
	if (MeleeAnimMontages.Num() > 1) { //has combo animations
		if (bIsAttacking) {
			if (bIsMeleeComboEnabled) {
				if (CurrentStepMeleeCombo < (MeleeAnimMontages.Num() - 1)) { //-1 because the last anim has no next stepCombo
					CurrentStepMeleeCombo++;
					SetMeleeComboEnabled(false); //don't make more than one combo in the same animation
				}
				else {
					return;
				}
			}
			else {
				return; //don't do anything else
			}
		}
	}
	else {
		if (bIsAttacking) return;
	}

	//Attack
	bIsAttacking = true;
	//Update damage multiplier of the weapon
	CurrentDamageMultiplier = StepComboDamageMultiplier * (CurrentStepMeleeCombo+1); //we use CurrentStepMeleeCombo+1 because currentStepMeleeCombo starts in zero
	
	if (CurrentStepMeleeCombo >= MeleeAnimMontages.Num()) UE_LOG(LogTemp, Error, TEXT("CurrentStepMeleeCombo is greater than the MeleeAnimMontages.Num"));
	PlayAnimMontageInOwner(MeleeAnimMontages[CurrentStepMeleeCombo]);
}

//Called from anim notifier when finished
void AUB_MeleeWeapon::OnFinishedAction()
{
	Super::OnFinishedAction();

	bIsAttacking = false;
}

//Additional Action, at this moment nope animation
void AUB_MeleeWeapon::StartAdditionalAction()
{
	Super::StartAdditionalAction();

	//Play a nope animation
	PlayAnimMontageInOwner(CurrentOwnerCharacter->DenyAnimMontage);
}

//Melee Detector
void AUB_MeleeWeapon::EnableMeleeDetector()
{
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
void AUB_MeleeWeapon::DisableMeleeDetector()
{
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
void AUB_MeleeWeapon::ResetMeleeCombo()
{
	SetMeleeComboEnabled(false);
	CurrentStepMeleeCombo = 0;
}
void AUB_MeleeWeapon::SetMeleeComboEnabled(bool NewState)
{
	bIsMeleeComboEnabled = NewState;
}
