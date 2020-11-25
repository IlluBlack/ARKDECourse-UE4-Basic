// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_FireVolume.generated.h"

class UBoxComponent;
class UParticleSystemComponent;

UCLASS()
class ARKDE_UE4BASIC_API AUB_FireVolume : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MainColliderComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* FireParticleComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireVolume")
	float TimeToApplyDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireVolume")
	float BurnDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireVolume")
	float LifeSpan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireVolume")
	TSubclassOf<UDamageType> DamageType;

	FTimerHandle BurnDamageTimer;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireVolume")
	FName AttachToSocketName;
	
public:	
	// Sets default values for this actor's properties
	AUB_FireVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ApplyBurnDamage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
