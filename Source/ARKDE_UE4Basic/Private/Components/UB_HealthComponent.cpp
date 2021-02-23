// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_HealthComponent.h"
#include "..\..\Public\Components\UB_HealthComponent.h"

#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UUB_HealthComponent::UUB_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
}


// Called when the game starts
void UUB_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetHealth();

	Owner = GetOwner();
	if (IsValid(Owner)) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UUB_HealthComponent::NotifyOnTakeDamage);
	}
}

void UUB_HealthComponent::ResetHealth()
{
	Health = MaxHealth;
}

void UUB_HealthComponent::NotifyOnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead) return;

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	if (Health <= 0.0f) {
		bIsDead = true;
		OnDeadDelegate.Broadcast(DamageCauser);
	}

	OnHealthChangedDelegate.Broadcast(this, DamagedActor, Damage * (-1), DamageType, InstigatedBy, DamageCauser);

	if (bDebug) {
		//UE_LOG(LogTemp, Log, TEXT("My health is %f"), Health);
		UE_LOG(LogTemp, Log, TEXT("My health is %s"), *FString::SanitizeFloat(Health));
	}
}

bool UUB_HealthComponent::TryGiveHealth(float ExtraHealth, AController* InstigatedBy, AActor* ActorCauser)
{
	if (bIsDead) return false;
	if (ExtraHealth <= 0.0f) return false;
	if (Health == MaxHealth) return false;

	Health = FMath::Clamp(Health + ExtraHealth, 0.0f, MaxHealth);
	OnHealthChangedDelegate.Broadcast(this, GetOwner(), Health, nullptr, InstigatedBy, ActorCauser);

	if (bDebug) {
		UE_LOG(LogTemp, Log, TEXT("My health is %s"), *FString::SanitizeFloat(Health));
	}

	return true;
}

