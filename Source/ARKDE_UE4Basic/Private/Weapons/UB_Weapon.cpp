// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Weapon.h"
#include "ARKDE_UE4Basic.h"
#include "UB_Character.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUB_Weapon::AUB_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent")); //returns pointer
	RootComponent = CustomRootComponent;

	PunchDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PunchDetectorComponent"));
	//PunchDetectorComponent->SetupAttachment(RootComponent); //I don't need it in a socket, assign in firearm or melee subclasses
	PunchDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	PunchDetectorComponent->SetCollisionResponseToChannel(OCC_ENEMY, ECR_Overlap);
	SetPunchDetectorEnabled(ECollisionEnabled::NoCollision); //starts with no collision

	Damage = 30.0f;
	PunchDamage = 18.0f;
}

// Called when the game starts or when spawned
void AUB_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	PunchDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AUB_Weapon::ApplyPunchDamage);
}

// Called every frame
void AUB_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Primary Action
void AUB_Weapon::StartAction() {
	BP_StartAction();
}
void AUB_Weapon::StopAction() {
	BP_StopAction();
}

//Additional Action

void AUB_Weapon::StartAdditionalAction() {
	BP_StartAdditionalAction();
}
void AUB_Weapon::StopAdditionalAction() {
	BP_StopAdditionalAction();
}

//Owner
void AUB_Weapon::SetCharacterOwner(AUB_Character* NewOwner)
{
	if (IsValid(NewOwner)) {
		SetOwner(NewOwner);
		CurrentOwnerCharacter = NewOwner;
	}
}

//PunchDetector
void AUB_Weapon::SetPunchDetectorEnabled(ECollisionEnabled::Type NewCollisionState)
{
	PunchDetectorComponent->SetCollisionEnabled(NewCollisionState);
}

void AUB_Weapon::ApplyPunchDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor)) {
		AActor* Owner = GetOwner();

		if (IsValid(Owner)) {
			UGameplayStatics::ApplyPointDamage(OtherActor, PunchDamage, SweepResult.Location, SweepResult, Owner->GetInstigatorController(), this, nullptr);
		}
	}
}


