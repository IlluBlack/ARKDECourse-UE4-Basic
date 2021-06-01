// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_BotPet.h"
#include "UB_Character.h"
#include "Enemy/UB_BotPetControlBase.h"
#include "Weapons/UB_Weapon.h"
#include "Components/UB_HealthComponent.h"
#include "Components/UB_ExplosionComponent.h"
#include "CollectableItems/UB_CollectableItem.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Engine/Public/TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

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

	BotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotMeshComponent"));
	BotMeshComponent->SetupAttachment(MainColliderComponent);
	BotMeshComponent->SetCanEverAffectNavigation(false);

	HealthComponent = CreateDefaultSubobject<UUB_HealthComponent>(TEXT("HealthComponent"));

	ExplosionComponent = CreateDefaultSubobject<UUB_ExplosionComponent>(TEXT("ExplosionComponent"));
	ExplosionComponent->bHasFalloffExplosion = false;
	ExplosionComponent->ExplosionMaxDamage = 100.0f;
	ExplosionComponent->ExplosionOuterRadius = 250.0f;
	ExplosionComponent->ExplosionDelay = 0.0f;

	MinDistanceToTarget = 100.0f;
	ForceMagnitude = 1000.0f;

	MaterialTimeParameterName = "Pulse";
	MaterialIndex = 0;

	XPValue = 15.0f;
	SpawnLootProbability = 100.0f;
}

// Called when the game starts or when spawned
void AUB_BotPet::BeginPlay()
{
	Super::BeginPlay();

	BotMaterial = BotMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(MaterialIndex, BotMeshComponent->GetMaterial(MaterialIndex));

	HealthComponent->OnHealthChangedDelegate.AddDynamic(this, &AUB_BotPet::OnHealthChanged);
	HealthComponent->OnDeadDelegate.AddDynamic(this, &AUB_BotPet::OnDead);

	ExplosionComponent->OnExplodeDelegate.AddDynamic(this, &AUB_BotPet::OnExplode);

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
		MoveToNextPathPoint();
	}

	if (bDrawNextPathPoint) {
		DrawDebugSphere(GetWorld(), NextPathPoint, 30.0f, 15.0f, FColor::Purple, false, 0.0f, 0, 1.0f);
	}
}

void AUB_BotPet::SetControlBase(AUB_BotPetControlBase* NewControlBase)
{
	ControlBase = NewControlBase;
}
void AUB_BotPet::OverrideLootSystem(TSubclassOf<AUB_CollectableItem> NewLootItemClass, float NewSpawnLootProbability)
{
	LootItemClass = NewLootItemClass;
	SpawnLootProbability = NewSpawnLootProbability;
}

//Navigation
FVector AUB_BotPet::GetNextPathPoint()
{
	if (IsValid(CurrentTargetCharacter)) {
		UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), CurrentTargetCharacter);
		if (NavigationPath->PathPoints.Num() > 1) { //0 is always the StartPosition
			return NavigationPath->PathPoints[1]; //nextPoint
		}
	}

	return GetActorLocation();
}
void AUB_BotPet::MoveToNextPathPoint()
{
	FVector ForceDirection = NextPathPoint - GetActorLocation();
	ForceDirection.Normalize();
	const FVector Force = ForceDirection * ForceMagnitude;

	MainColliderComponent->AddForce(Force, NAME_None, true);
}

//Taking damage
void AUB_BotPet::OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Value, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(BotMaterial) && Value < 0.0f) {
		BotMaterial->SetScalarParameterValue(MaterialTimeParameterName, GetWorld()->TimeSeconds);
	}
}
void AUB_BotPet::OnDead(AActor* ActorCauser)
{
	//Give Reward
	AUB_Character* CharacterCauser = Cast<AUB_Character>(ActorCauser);
	if (!IsValid(CharacterCauser)) {
		AUB_Weapon* WeaponCauser = Cast<AUB_Weapon>(ActorCauser);
		if (IsValid(WeaponCauser)) {
			CharacterCauser = WeaponCauser->GetOwnerCharacter();
		}
	}

	if (IsValid(CharacterCauser) && CharacterCauser->GetCharacterType() == EUB_CharacterType::CharacterType_Player) {
		CharacterCauser->EarnUltimateXP(XPValue);
		TrySpawnLoot();
	}

	SelfDestruction();
}

//Destruction
void AUB_BotPet::SelfDestruction()
{
	if (bHasExploded) return;

	const TArray<AActor*> IgnoreActors = TArray<AActor*>{ this };
	ExplosionComponent->StartExplosion(IgnoreActors);
	bHasExploded = true;
}
void AUB_BotPet::OnExplode(UUB_ExplosionComponent* CurrentExplosionComponent, const TArray<AActor*> OverlappedActors) {
	Destroy();
}

void AUB_BotPet::Destroyed() {
	if (IsValid(ControlBase)) {
		ControlBase->UnsubscribeBotPet(this);
	}
}

//Loot
bool AUB_BotPet::TrySpawnLoot()
{
	if (!IsValid(LootItemClass)) return false;

	float SelectedProbability = FMath::RandRange(0.0f, 100.0f);
	if (SelectedProbability <= SpawnLootProbability) {
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AUB_CollectableItem>(LootItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
		return true;
	}

	return false;
}

