// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_DoorKey.h"
#include "UB_Character.h"
#include "UB_CharacterInventory.h"
#include "Components/StaticMeshComponent.h"

AUB_DoorKey::AUB_DoorKey()
{
	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KeyTag = "KeyA";
}

void AUB_DoorKey::Pickup(AUB_Character* PickupCharacter)
{
	Super::Pickup(PickupCharacter);

	//PickupCharacter->AddKey(KeyTag);
	PickupCharacter->Inventory->AddKey(KeyTag);
	Destroy();
}