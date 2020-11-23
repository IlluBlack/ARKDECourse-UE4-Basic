// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_SwitcherLaunchPads.h"
#include "Components/StaticMeshComponent.h"
#include "UB_LaunchPad.h"
#include "Components/BoxComponent.h"

AUB_SwitcherLaunchPads::AUB_SwitcherLaunchPads()
{
	//Physic collision with world
	MainColliderComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	SwitcherMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switcher"));
	SwitcherMeshComponent->SetupAttachment(RootComponent);

	DurationLaunchPadsActivation = 8.0f;
	bIsOn = true;
}

// Called when the game starts or when spawned
void AUB_SwitcherLaunchPads::BeginPlay()
{
	Super::BeginPlay();

	ResetLaunchPadsState();
}

// Called every frame
void AUB_SwitcherLaunchPads::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOn) {
		if (CurrentActiveTime >= DurationLaunchPadsActivation) {
			bIsOn = false;
			ResetLaunchPadsState();
		}
		else {
			CurrentActiveTime += DeltaTime;
		}
	}
}

void AUB_SwitcherLaunchPads::OnCharacterOverlap(AUB_Character* Character)
{
	Super::OnCharacterOverlap(Character);

	CurrentActiveTime = 0.0f; //Reset timer
	if (bIsOn) return; //if it's on, just reset timer

	bIsOn = true;
	ResetLaunchPadsState();
}

void AUB_SwitcherLaunchPads::ResetLaunchPadsState()
{
	for(AUB_LaunchPad* launchPad: ControlLaunchPads)
	{
		if (IsValid(launchPad)) {
			launchPad->SetActive(bIsOn);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("LauchPad assigned to ControlLauchPads is not valid"));
		}
	}
}