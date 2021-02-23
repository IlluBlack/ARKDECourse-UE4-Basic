// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_ProximityMine.h"
#include "Components/UB_ExplosionComponent.h"
#include "Components/UB_HealthComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUB_ProximityMine::AUB_ProximityMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MineMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mine"));
	MineMeshComponent->SetupAttachment(RootComponent);

	ExplosionComponent = CreateDefaultSubobject<UUB_ExplosionComponent>(TEXT("ExplosionComponent"));
	HealthComponent = CreateDefaultSubobject<UUB_HealthComponent>(TEXT("HealthComponent"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;

	const TArray<AActor*> IgnoreActors = { this };

	DisappearDelay = 0.2f;
	DistanceForWarning = 200.0f;
	DistanceForActivation = 100.0f;
	TimeToVerifyActors = 0.2f;
}

// Called when the game starts or when spawned
void AUB_ProximityMine::BeginPlay()
{
	Super::BeginPlay();
	
	if (DistanceForWarning <= DistanceForActivation) UE_LOG(LogTemp, Error, TEXT("DistanceForActivation should be less than DistanceForWarning"));
	
	HealthComponent->OnHealthChangedDelegate.AddDynamic(this, &AUB_ProximityMine::OnHealthChanged);
	ExplosionComponent->OnExplodeDelegate.AddDynamic(this, &AUB_ProximityMine::OnExplode);

	GetWorldTimerManager().SetTimer(VerifyActorsTimer, this, &AUB_ProximityMine::VerifyActorsInRange, TimeToVerifyActors, true, TimeToVerifyActors);
}

// Called every frame
void AUB_ProximityMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUB_ProximityMine::VerifyActorsInRange()
{
	FVector Center = GetActorLocation();

	const TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes = {
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn),
	};

	TArray<AActor*> OutActors;
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Center, DistanceForWarning, traceObjectTypes, nullptr, IgnoreActors, OutActors);

	if (bHit) {
		for (AActor* overlappedActor : OutActors) {
			if (IsValid(overlappedActor)) {
				//Verify if it's close enough to explode
				float Distance = (Center - overlappedActor->GetActorLocation()).Size();

				if (Distance <= DistanceForActivation) {
					ActivateMine();
				}
				else {
					ProximityWarn();
				}
			}
		}
	}

	if (bDebugAreaVerify) {
		DrawDebugSphere(GetWorld(), Center, DistanceForWarning, 20, FColor::White, false, 1.0f);
		DrawDebugSphere(GetWorld(), Center, DistanceForActivation, 20, FColor::Red, false, 1.0f);
	}
}

void AUB_ProximityMine::ProximityWarn()
{
	AudioComponent->Play();
}

void AUB_ProximityMine::ActivateMine()
{
	//Stop loop timer
	GetWorldTimerManager().ClearTimer(VerifyActorsTimer);
	ExplosionComponent->StartExplosion(IgnoreActors);
}

void AUB_ProximityMine::OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Value, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->IsDead()) {
		ActivateMine();
	}
}

void AUB_ProximityMine::OnExplode(UUB_ExplosionComponent* CurrentExplosionComponent, const TArray<AActor*> OverlappedActors)
{
	SetLifeSpan(DisappearDelay);
}


