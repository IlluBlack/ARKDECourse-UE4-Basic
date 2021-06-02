// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_InteractiveItem.h"

#include "Engine/Classes/Engine/World.h"
#include "Engine/Public/TimerManager.h"

// Sets default values
AUB_InteractiveItem::AUB_InteractiveItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HoldTimeToFireInteraction = 0.0f;
}

// Called when the game starts or when spawned
void AUB_InteractiveItem::BeginPlay()
{
	Super::BeginPlay();

	bIsInteracting = false;
}

// Called every frame
void AUB_InteractiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AUB_InteractiveItem::CanStartInteraction(AActor* ActorInteracting)
{
	if (bIsInteracting) return false;
	if (!IsValid(ActorInteracting)) return false;

	return true;
}

void AUB_InteractiveItem::FireInteraction(AActor* ActorInteracting) {
	BP_FireInteraction(ActorInteracting);
}

void AUB_InteractiveItem::StartInteraction_Implementation(AActor* ActorInteracting)
{
	if (!CanStartInteraction(ActorInteracting)) return;

	bIsInteracting = true;

	if (HoldTimeToFireInteraction <= 0.0f) {
		FireInteraction(ActorInteracting);
	}
	else {
		TimerDelegate_HoldInteraction = FTimerDelegate::CreateUObject(this, &AUB_InteractiveItem::FireInteraction, ActorInteracting);
		GetWorldTimerManager().SetTimer(TimerHandle_HoldInteraction, TimerDelegate_HoldInteraction, HoldTimeToFireInteraction, false);
	}

	Execute_BP_StartInteraction(this, ActorInteracting);
}

void AUB_InteractiveItem::StopInteraction_Implementation(AActor* ActorInteracting)
{
	if (!bIsInteracting) return;

	bIsInteracting = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_HoldInteraction);

	Execute_BP_StopInteraction(this, ActorInteracting);
}

