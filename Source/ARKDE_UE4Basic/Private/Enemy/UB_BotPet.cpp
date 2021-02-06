// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_BotPet.h"
#include "UB_Character.h"
#include "Components/UB_HealthComponent.h"
#include "Components/UB_ExplosionComponent.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Engine/Public/TimerManager.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUB_BotPet::AUB_BotPet()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MainColliderComponent"));
	RootComponent = MainColliderComponent;

	MainColliderComponent->SetCanEverAffectNavigation(false);
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainColliderComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MainColliderComponent->SetSimulatePhysics(true);

	BotMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BotMeshComponent"));
	BotMeshComponent->SetupAttachment(MainColliderComponent);

	HealthComponent = CreateDefaultSubobject<UUB_HealthComponent>(TEXT("HealthComponent"));

	SelfDestructionDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SelfDestructionDetectorComponent"));
	SelfDestructionDetectorComponent->SetupAttachment(MainColliderComponent);
	SelfDestructionDetectorComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SelfDestructionDetectorComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SelfDestructionDetectorComponent->SetSphereRadius(150.0f);

	ExplosionComponent = CreateDefaultSubobject<UUB_ExplosionComponent>(TEXT("ExplosionComponent"));
	ExplosionComponent->bHasFalloffExplosion = false;
	ExplosionComponent->ExplosionMaxDamage = 100.0f;
	ExplosionComponent->ExplosionOuterRadius = 250.0f;
	ExplosionComponent->ExplosionDelay = 0.0f;

	MinDistanceToTarget = 100.0f;
	ForceMagnitude = 1000.0f;

	SelfDestructionCountdownFrequency = 0.5f;
	SelfDestructionCountdownDamage = 20.0f;

	MaterialTimeParameterName = "Pulse";
	MaterialIndex = 0;
}

// Called when the game starts or when spawned
void AUB_BotPet::BeginPlay()
{
	Super::BeginPlay();

	BotMaterial = BotMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(MaterialIndex, BotMeshComponent->GetMaterial(MaterialIndex));
	HealthComponent->OnHealthChangedDelegate.AddDynamic(this, &AUB_BotPet::OnHealthChanged);
	SelfDestructionDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AUB_BotPet::OnOtherActorsDetection);
	ExplosionComponent->OnExplodeDelegate.AddDynamic(this, &AUB_BotPet::OnExplode);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn)) {
		PlayerCharacter = Cast<AUB_Character>(PlayerPawn);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn not found"));
	}

	NextPathPoint = GetNextPathPoint();
}

// Called every frame
void AUB_BotPet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Move to path point
	const float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
	if (DistanceToTarget <= MinDistanceToTarget) { //arrived to the target point
		NextPathPoint = GetNextPathPoint();
	}
	else {
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		const FVector Force = ForceDirection * ForceMagnitude;

		MainColliderComponent->AddForce(Force, NAME_None, true);
	}

	if (bDrawNextPathPoint) {
		DrawDebugSphere(GetWorld(), NextPathPoint, 30.0f, 15.0f, FColor::Purple, false, 0.0f, 0, 1.0f);
	}
}

//Navigation
FVector AUB_BotPet::GetNextPathPoint()
{
	if (IsValid(PlayerCharacter)) {
		UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), PlayerCharacter);
		if (NavigationPath->PathPoints.Num() > 1) { //0 is always the StartPosition
			return NavigationPath->PathPoints[1]; //nextPoint
		}
	}

	return GetActorLocation();
}

//Taking damage
void AUB_BotPet::OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(BotMaterial)) {
		BotMaterial->SetScalarParameterValue(MaterialTimeParameterName, GetWorld()->TimeSeconds);
	}

	if (HealthComponent->IsDead()) {
		SelfDestruction();
	}
}

//SelfDestructionDetection
void AUB_BotPet::OnOtherActorsDetection(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == PlayerCharacter) {
		if (bStartedSelfDestructionCountdown) return;
		bStartedSelfDestructionCountdown = true;

		GetWorldTimerManager().SetTimer(TimerHandle_SelfDestructionCountdown, this, &AUB_BotPet::SelfDamage, SelfDestructionCountdownFrequency, true);
	}
}
void AUB_BotPet::SelfDamage() 
{
	UGameplayStatics::ApplyDamage(this, SelfDestructionCountdownDamage, GetInstigatorController(), nullptr, nullptr);
}

//Destruction
void AUB_BotPet::SelfDestruction()
{
	if (bIsExploded) return;

	const TArray<AActor*> IgnoreActors = TArray<AActor*>{ this };
	ExplosionComponent->StartExplosion(IgnoreActors);
	bIsExploded = true;
}
void AUB_BotPet::OnExplode(UUB_ExplosionComponent* CurrentExplosionComponent, const TArray<AActor*> OverlappedActors)
{
	Destroy();
}

