// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_FireVolume.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUB_FireVolume::AUB_FireVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MainColliderComponent"));
	MainColliderComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MainColliderComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = MainColliderComponent;

	FireParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticleComponent"));
	FireParticleComponent->SetupAttachment(MainColliderComponent);
	
	TimeToApplyDamage = 1.0f; //DPS
	BurnDamage = 10.0f;
	LifeSpan = 5.0f;

	AttachToSocketName = "SCK_Fire";
}

// Called when the game starts or when spawned
void AUB_FireVolume::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(LifeSpan);
	GetWorldTimerManager().SetTimer(BurnDamageTimer, this, &AUB_FireVolume::ApplyBurnDamage, TimeToApplyDamage, true, TimeToApplyDamage);
}

// Called every frame
void AUB_FireVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUB_FireVolume::ApplyBurnDamage()
{
	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors);

	for (AActor* OverlappedActor : OverlappedActors) {
		if (IsValid(OverlappedActor)) {
			UGameplayStatics::ApplyDamage(OverlappedActor, BurnDamage, GetInstigatorController(), this, DamageType);
		}
	}
}

