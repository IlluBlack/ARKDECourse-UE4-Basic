// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Weapon.h"
#include "GameFramework/Character.h"

// Sets default values
AUB_Weapon::AUB_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20.0f;
}

// Called when the game starts or when spawned
void AUB_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUB_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUB_Weapon::StartAction()
{
	BP_StartAction();
}

void AUB_Weapon::StopAction()
{
	BP_StopAction();
}

void AUB_Weapon::SetCharacterOwner(ACharacter* NewOwner)
{
	if (IsValid(NewOwner)) {
		SetOwner(NewOwner);
		CurrentOwnerCharacter = NewOwner;
	}
}

