// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_ExplosiveBarrel.h"
#include "UB_FireVolume.h"
#include "Components/UB_ExplosionComponent.h"
#include "Components/UB_HealthComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine/World.h"

// Sets default values
AUB_ExplosiveBarrel::AUB_ExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel"));
	BarrelMeshComponent->SetupAttachment(RootComponent);

	ExplosionComponent = CreateDefaultSubobject<UUB_ExplosionComponent>(TEXT("ExplosionComponent"));
	HealthComponent = CreateDefaultSubobject<UUB_HealthComponent>(TEXT("HealthComponent"));
	
	const TArray<AActor*> IgnoreActors = { this };
	DisappearDelay = 0.2f;
}

// Called when the game starts or when spawned
void AUB_ExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnHealthChangedDelegate.AddDynamic(this, &AUB_ExplosiveBarrel::OnHealthChanged);
	ExplosionComponent->OnExplodeDelegate.AddDynamic(this, &AUB_ExplosiveBarrel::OnExplode);
}

// Called every frame
void AUB_ExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUB_ExplosiveBarrel::OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->IsDead()) {
		ExplosionComponent->StartExplosion(IgnoreActors);
	}
}

void AUB_ExplosiveBarrel::OnExplode(UUB_ExplosionComponent* CurrentExplosionComponent, const TArray<AActor*> OverlappedActors)
{
	//Set fire volume on actors that affected with explosion
	if (IsValid(FireVolumeClass)) {
		for (AActor* OverlappedActor : OverlappedActors) {
			if (IsValid(OverlappedActor)) {
				AUB_FireVolume* Fire = GetWorld()->SpawnActor<AUB_FireVolume>(FireVolumeClass, OverlappedActor->GetActorLocation(), OverlappedActor->GetActorRotation());
				if (IsValid(Fire)) {
					Fire->AttachToActor(OverlappedActor, FAttachmentTransformRules::KeepWorldTransform, Fire->AttachToSocketName);
					//Fire->AttachToComponent(OverlappedActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Fire->AttachToSocketName);
				}
			}
		}
	}
	
	SetLifeSpan(DisappearDelay);
}


