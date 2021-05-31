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

	bUseAnimMontageSections = true;
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
	if (CurrentState != EUB_WeaponState::State_Idle &&
		CurrentState != EUB_WeaponState::State_Action) return;

	//the melee collider is enabled in the anim notify
	if (GetNumCombos() > 1) { //has combo animations
		if (CurrentState == EUB_WeaponState::State_Action) { //isAttacking
			if (bIsMeleeComboEnabled) {
				if (CurrentStepMeleeCombo < (GetNumCombos() - 1)) { //-1 because the last anim has no next stepCombo
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
		if (CurrentState == EUB_WeaponState::State_Action) return;
	}

	Super::StartAction();

	//Update damage multiplier of the weapon
	CurrentDamageMultiplier = StepComboDamageMultiplier * (CurrentStepMeleeCombo+1); //we use CurrentStepMeleeCombo+1 because currentStepMeleeCombo starts in zero
	
	if (CurrentStepMeleeCombo >= GetNumCombos()) UE_LOG(LogTemp, Error, TEXT("CurrentStepMeleeCombo is greater than the MeleeAnimMontages or AnimMontageSections"));
	
	//Play Animation
	if (bUseAnimMontageSections) {
		PlayAnimMontageInOwner(MeleeAnimMontageWithSections, 1.0f, AnimMontageSections[CurrentStepMeleeCombo]);
	}
	else {
		PlayAnimMontageInOwner(MeleeAnimMontagesWithoutSections[CurrentStepMeleeCombo]); //play in the array of animMontages
	}
}

//Additional Action, at this moment nope animation
void AUB_MeleeWeapon::StartAdditionalAction()
{
	if (CurrentState != EUB_WeaponState::State_Idle) return;

	//Play a nope animation, do not change state
	PlayAnimMontageInOwner(CurrentOwnerCharacter->DenyAnimMontage);
	BP_StartAdditionalAction();
}

//Punch action
void AUB_MeleeWeapon::StartPunchAction()
{
	if (CurrentState == EUB_WeaponState::State_PunchAction) return;

	Super::StartPunchAction();
}

//Melee Detector
void AUB_MeleeWeapon::EnableMeleeDetector()
{
	bAppliedDamage = false; //reset appliedDamage
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
void AUB_MeleeWeapon::DisableMeleeDetector()
{
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AUB_MeleeWeapon::ApplyMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bAppliedDamage) return;

	if (IsValid(OtherActor)) {
		AActor* Owner = GetOwner();

		if (IsValid(Owner) && (OtherActor != Owner)) 
		{
			AUB_Character* OtherActorCharacter = Cast<AUB_Character>(OtherActor);
			if (IsValid(OtherActorCharacter)) {
				if (OtherActorCharacter->GetCharacterType() == CurrentOwnerCharacter->GetCharacterType()) return;
			}

			UGameplayStatics::ApplyPointDamage(OtherActor, Damage * CurrentDamageMultiplier, SweepResult.Location, SweepResult, Owner->GetInstigatorController(), this, nullptr);
			bAppliedDamage = true;
		}
	}
}

//Combos
int AUB_MeleeWeapon::GetNumCombos()
{
	if (bUseAnimMontageSections) {
		return AnimMontageSections.Num();
	}
	
	return MeleeAnimMontagesWithoutSections.Num();
}
void AUB_MeleeWeapon::ResetMeleeCombo()
{
	SetMeleeComboEnabled(false);
	CurrentStepMeleeCombo = 0;
}
void AUB_MeleeWeapon::SetMeleeComboEnabled(bool NewState)
{
	bIsMeleeComboEnabled = NewState;
}
