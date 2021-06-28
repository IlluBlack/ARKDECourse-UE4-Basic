// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Enemy.h"
#include "UB_Character.h"
#include "Weapons/UB_Weapon.h"
#include "CollectableItems/UB_CollectableItem.h"
#include "Enemy/UB_AICEnemy.h"
#include "Components/UB_HealthComponent.h"
#include "Core/UB_GameInstance.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "AIModule/Classes/Perception/AISense_Damage.h"

AUB_Enemy::AUB_Enemy()
{
	Accuracy = 0.4f;
	AimError = 2.5f;

	XPValue = 20.0f;
	SpawnLootProbability = 100.0f;

	bLoopPath = false;
	DirectionIndexPath = 1;
	WaitingTimeOnPathPoint = 1.0f;
}

void AUB_Enemy::BeginPlay()
{
	Super::BeginPlay();

	AIEnemyController = Cast<AUB_AICEnemy>(GetController());
}

void AUB_Enemy::SetLongRangeTargetPoint(AUB_Character* Target)
{
	if (IsValid(Target)) {
		if (Target->BodyTargetsSocketName.Num() <= 0) {
			LongRangeTargetPoint = Target->GetTargetLocation();
			return;
		}

		const int BodySocketIndex = FMath::RandRange(0, Target->BodyTargetsSocketName.Num()-1);
		const FName BodySocketName = Target->BodyTargetsSocketName[BodySocketIndex];

		const FVector BodySocketLocation = Target->GetMesh()->GetSocketLocation(BodySocketName);
		const FVector AimErrorLocation = FVector(
			FMath::RandRange(-AimError, +AimError),
			FMath::RandRange(-AimError, +AimError),
			FMath::RandRange(-AimError, +AimError)
		);
		LongRangeTargetPoint = BodySocketLocation + AimErrorLocation;

		//UE_LOG(LogTemp, Log, TEXT("- BodySocketLocation %s"), *BodySocketLocation.ToString());
		//UE_LOG(LogTemp, Log, TEXT("ShotPoint %s"), *LongRangeTargetPoint.ToString());
	}
}

FRotator AUB_Enemy::GetViewRotation() const
{
	if (LongRangeTargetPoint != FVector::ZeroVector) {
		return UKismetMathLibrary::FindLookAtRotation(GetPawnViewLocation(), LongRangeTargetPoint);
	}

	return Super::GetViewRotation();
}

void AUB_Enemy::OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Value, const UDamageType* DamageType, AController* InstigatedBy, AActor* ActorCauser)
{
	Super::OnHealthChanged(CurrentHealthComponent, DamagedActor, Value, DamageType, InstigatedBy, ActorCauser);

	if (!IsValid(AIEnemyController)) return;

	if (Value <= 0) //received damage
	{
		//Manually notify to AIController that received damage
		AUB_Weapon* WeaponCauser = Cast<AUB_Weapon>(ActorCauser);
		if (IsValid(WeaponCauser)) {
			AIEnemyController->SetReceivingDamage(true);
			AActor* OwnerCauser = WeaponCauser->GetOwner();
			if (IsValid(OwnerCauser)) {
				UAISense_Damage::ReportDamageEvent(GetWorld(), this, OwnerCauser, Value, OwnerCauser->GetActorLocation(), FVector::ZeroVector);
			}
		}
	}
}

void AUB_Enemy::OnDead(AActor* ActorCauser)
{
	Super::OnDead(ActorCauser);

	AUB_Character* CharacterCauser = Cast<AUB_Character>(ActorCauser);
	if (!IsValid(CharacterCauser)) {
		//verify if causer is a weapon
		AUB_Weapon* WeaponCauser = Cast<AUB_Weapon>(ActorCauser);
		if (IsValid(WeaponCauser)) {
			CharacterCauser = WeaponCauser->GetOwnerCharacter();
		}
	}

	if (IsValid(CharacterCauser) && CharacterCauser->GetCharacterType() == EUB_CharacterType::CharacterType_Player) {
		CharacterCauser->EarnUltimateXP(XPValue);
		TrySpawnLoot();
		if (IsValid(GameInstanceReference)) {
			GameInstanceReference->AddDefeatedEnemies();
		}
	}

	if (IsValid(AIEnemyController)) {
		AIEnemyController->UnPossess();
	}
}

bool AUB_Enemy::TrySpawnLoot()
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
