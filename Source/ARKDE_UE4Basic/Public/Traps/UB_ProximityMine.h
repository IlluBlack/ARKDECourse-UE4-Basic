// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_ProximityMine.generated.h"

class UStaticMeshComponent;
class UUB_ExplosionComponent;
class UUB_HealthComponent;
class UAudioComponent;

UCLASS()
class ARKDE_UE4BASIC_API AUB_ProximityMine : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MineMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUB_ExplosionComponent* ExplosionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUB_HealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProximityMine")
	float DistanceForWarning;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProximityMine")
	float DistanceForActivation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProximityMine")
	float TimeToVerifyActors;
	FTimerHandle VerifyActorsTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProximityMine")
	float DisappearDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebugAreaVerify;

	const TArray<AActor*> IgnoreActors;

public:	
	// Sets default values for this actor's properties
	AUB_ProximityMine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void VerifyActorsInRange();
	void ProximityWarn();
	void ActivateMine();

	UFUNCTION()
	void OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnExplode(UUB_ExplosionComponent* CurrentExplosionComponent, const TArray<AActor*> OverlappedActors);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
