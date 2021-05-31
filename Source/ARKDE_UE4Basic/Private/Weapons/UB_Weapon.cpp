// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Weapon.h"
#include "ARKDE_UE4Basic.h"
#include "UB_Character.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"

// Sets default values
AUB_Weapon::AUB_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent")); //returns pointer
	RootComponent = CustomRootComponent;

	PunchDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PunchDetectorComponent"));
	//PunchDetectorComponent->SetupAttachment(RootComponent); //I don't need it in a socket, assign in firearm or melee subclasses
	SetupPunchDetector();
	DisablePunchDetector();

	Damage = 30.0f;
	PunchDamage = 18.0f;
}

// Called when the game starts or when spawned
void AUB_Weapon::BeginPlay()
{
	Super::BeginPlay();

	PunchDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AUB_Weapon::ApplyPunchDamage);
}

void AUB_Weapon::EquipWeapon() 
{
	CurrentState = EUB_WeaponState::State_Idle;
	SetupPunchDetector();
}

void AUB_Weapon::SetupPunchDetector()
{
	PunchDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	if (IsValid(CurrentOwnerCharacter) && CurrentOwnerCharacter->GetCharacterType() == EUB_CharacterType::CharacterType_Player) {
		PunchDetectorComponent->SetCollisionResponseToChannel(OCC_ENEMY, ECR_Overlap);
	}
	else {
		PunchDetectorComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	}
}

// Called every frame
void AUB_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Primary Action
void AUB_Weapon::StartAction()
{
	//if (bIsPunching) return;
	//if (CurrentState != EUB_WeaponState::State_Idle) return false;
	//overrride and add validators if needed
	bIsWeaponActionTriggered = true;
	CurrentState = EUB_WeaponState::State_Action;
	BP_StartAction();
}
void AUB_Weapon::StopAction() {
	bIsWeaponActionTriggered = false;
	BP_StopAction();
}
//Called from anim notifier when finished
void AUB_Weapon::OnFinishedAction() {
	CurrentState = EUB_WeaponState::State_Idle;
}

//Additional Action
void AUB_Weapon::StartAdditionalAction() 
{
	//if (bIsPunching) return;
	//if (CurrentState != EUB_WeaponState::State_Idle) return false;

	CurrentState = EUB_WeaponState::State_AdditionalAction;
	BP_StartAdditionalAction();
}
void AUB_Weapon::OnFinishedAdditionalAction()
{
	CurrentState = EUB_WeaponState::State_Idle;
}

//Punch Action
void AUB_Weapon::StartPunchAction()
{
	//if (bIsPunching) return;
	//if (CurrentState != EUB_WeaponState::State_Idle) return false;

	CurrentState = EUB_WeaponState::State_PunchAction;
	PlayAnimMontageInOwner(PunchAnimMontage);
}
void AUB_Weapon::OnFinishedPunchAction()
{
	//bIsPunching = false;
	CurrentState = EUB_WeaponState::State_Idle;
}
void AUB_Weapon::EnablePunchDetector()
{
	PunchDetectorComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
void AUB_Weapon::DisablePunchDetector()
{
	PunchDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

//Punch Detector
void AUB_Weapon::ApplyPunchDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor)) {
		AActor* Owner = GetOwner();

		if (IsValid(Owner) && (OtherActor != Owner)) 
		{
			AUB_Character* OtherActorCharacter = Cast<AUB_Character>(OtherActor);
			if (IsValid(OtherActorCharacter)) {
				if (OtherActorCharacter->GetCharacterType() == CurrentOwnerCharacter->GetCharacterType()) return; //enemy dont make damage to enemies
			}

			UGameplayStatics::ApplyPointDamage(OtherActor, PunchDamage, SweepResult.Location, SweepResult, Owner->GetInstigatorController(), this, nullptr);
		}
	}
}

//Owner
void AUB_Weapon::PlayAnimMontageInOwner(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	if (IsValid(CurrentOwnerCharacter) && IsValid(AnimMontage)) {
		CurrentOwnerCharacter->PlayAnimMontage(AnimMontage, false, InPlayRate, StartSectionName);
	}
}
void AUB_Weapon::StopAnimMontageInOwner(UAnimMontage* AnimMontage)
{
	if (IsValid(CurrentOwnerCharacter) && IsValid(AnimMontage)) {
		CurrentOwnerCharacter->StopAnimMontage(AnimMontage);
	}
}

void AUB_Weapon::SetCharacterOwner(AUB_Character* NewOwner)
{
	if (IsValid(NewOwner)) {
		SetOwner(NewOwner);
		CurrentOwnerCharacter = NewOwner;
	}
}
