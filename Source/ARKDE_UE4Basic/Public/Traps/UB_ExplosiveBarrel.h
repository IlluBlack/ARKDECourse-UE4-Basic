// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_ExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class UUB_ExplosionComponent;
class UUB_HealthComponent;
class AUB_FireVolume;

UCLASS()
class ARKDE_UE4BASIC_API AUB_ExplosiveBarrel : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BarrelMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUB_ExplosionComponent* ExplosionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUB_HealthComponent* HealthComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExplosiveBarrel")
	TSubclassOf<AUB_FireVolume> FireVolumeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ExplosiveBarrel")
	float DisappearDelay;

	const TArray<AActor*> IgnoreActors;
	
public:	
	// Sets default values for this actor's properties
	AUB_ExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnExplode(UUB_ExplosionComponent* CurrentExplosionComponent, const TArray<AActor*> OverlappedActors);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
