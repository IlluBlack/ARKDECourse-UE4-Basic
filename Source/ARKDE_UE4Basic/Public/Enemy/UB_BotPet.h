// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UB_BotPet.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class AUB_Character;
class UUB_HealthComponent;
class UUB_ExplosionComponent;
class AUB_CollectableItem;
class AUB_BotPetControlBase;

UCLASS()
class ARKDE_UE4BASIC_API AUB_BotPet : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* MainColliderComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BotMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUB_HealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUB_ExplosionComponent* ExplosionComponent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector NextPathPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Debug")
	bool bDrawNextPathPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MinDistanceToTarget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float ForceMagnitude;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Self Destruction")
	bool bIsExploded;

	UMaterialInstanceDynamic* BotMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Material")
	FName MaterialTimeParameterName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Material")
	int MaterialIndex;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	AUB_Character* CurrentTargetCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate XP")
	float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Sytem")
	TSubclassOf<AUB_CollectableItem> LootItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Sytem")
	float SpawnLootProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Base")
	AUB_BotPetControlBase* ControlBase;

public:
	// Sets default values for this pawn's properties
	AUB_BotPet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	virtual FVector GetNextPathPoint();
	void MoveToNextPathPoint();

	UFUNCTION()
	void OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION()
	virtual void OnDead(AActor* ActorCauser);

	void SelfDestruction();
	UFUNCTION()
	void OnExplode(UUB_ExplosionComponent* CurrentExplosionComponent, const TArray<AActor*> OverlappedActors);

	bool TrySpawnLoot();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	void SetControlBase(AUB_BotPetControlBase* NewControlBase);
	void OverrideLootSystem(TSubclassOf<AUB_CollectableItem> NewLootItemClass, float NewSpawnLootProbability);

};
