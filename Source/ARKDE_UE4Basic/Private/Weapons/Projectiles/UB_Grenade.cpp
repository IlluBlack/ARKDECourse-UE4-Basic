// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Grenade.h"
#include "Components/UB_ExplosionComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"

AUB_Grenade::AUB_Grenade()
{
	ExplosionComponent = CreateDefaultSubobject<UUB_ExplosionComponent>(TEXT("ExplosionComponent"));
	DisappearDelay = 1.0f;
}

void AUB_Grenade::BeginPlay()
{
	Super::BeginPlay();

	ExplosionComponent->OnExplodeDelegate.AddDynamic(this, &AUB_Grenade::OnExplode);
}

void AUB_Grenade::Throw()
{
	const TArray<AActor*> IgnoreActors = {}; //at this moment 
	ExplosionComponent->StartExplosion(IgnoreActors);
}

void AUB_Grenade::OnExplode(UUB_ExplosionComponent* CurrentExplosionComponent, const TArray<AActor*> OverlappedActors)
{
	GetWorldTimerManager().SetTimer(DisappearDelayTimer, this, &AUB_Grenade::Disappear, DisappearDelay, false);
}

void AUB_Grenade::Disappear()
{
	GetWorldTimerManager().ClearTimer(DisappearDelayTimer);
	Destroy();
}