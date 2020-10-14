// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_LaunchPad.h"
#include "UB_Character.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"

AUB_LaunchPad::AUB_LaunchPad()
{
	LauchPadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LauchPad"));
	LauchPadMeshComponent->SetupAttachment(RootComponent);

	ActiveMaterial = CreateDefaultSubobject<UMaterial>(TEXT("ActiveMaterial"));
	DisableMaterial = CreateDefaultSubobject<UMaterial>(TEXT("DisableMaterial"));

	bIsActive = false;
	LaunchVelocity = FVector(0, 0, 900);
	bXYOverride = false;
	bZOverride = false;
}

// Called when the game starts or when spawned
void AUB_LaunchPad::BeginPlay()
{
	Super::BeginPlay();
	ResetMaterial();
}

void AUB_LaunchPad::SetActive(bool bSetActive)
{
	bIsActive = bSetActive;
	ResetMaterial();
}

void AUB_LaunchPad::ResetMaterial()
{
	LauchPadMeshComponent->SetMaterial(0, bIsActive ? ActiveMaterial : DisableMaterial);
}

void AUB_LaunchPad::OnCharacterOverlap(AUB_Character* Character)
{
	if (!bIsActive) return;

	Super::OnCharacterOverlap(Character);

	Character->LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}

