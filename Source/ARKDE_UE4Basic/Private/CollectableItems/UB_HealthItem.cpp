// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_HealthItem.h"
#include "UB_Character.h"

#include "Components/StaticMeshComponent.h"

AUB_HealthItem::AUB_HealthItem()
{
	HealthItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthItemMeshComponent"));
	HealthItemMeshComponent->SetupAttachment(RootComponent);
	HealthItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	XPValue = 0.0f;
	HealthValue = 15.0f;
}

void AUB_HealthItem::Pickup(AUB_Character* PickupCharacter)
{
	bool WasUsed = false;
	if (IsValid(PickupCharacter)) {
		WasUsed = PickupCharacter->TryGiveHealth(HealthValue, GetInstigatorController(), this);
	}

	BP_Pickup(PickupCharacter);

	if (bShouldDestroyWhenPickedUp && WasUsed)
		Destroy();
}
