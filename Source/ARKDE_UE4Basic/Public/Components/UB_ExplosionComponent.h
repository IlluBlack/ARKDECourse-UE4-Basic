// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UB_ExplosionComponent.generated.h"

//Delegate signature
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExplodeSignature, UUB_ExplosionComponent*, ExplosionComponent, const TArray<AActor*>, OverlappedActors);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARKDE_UE4BASIC_API UUB_ExplosionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Component")
	TSubclassOf<UDamageType> DamageType; //if there's not assigned, it takes the default one

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Component|Debug")
	bool bDebugExplosion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Component|Effects")
	UParticleSystem* ExplosionEffect;

	FTimerHandle ExplosionDelayTimer;

	TArray<AActor*> IgnoreActors;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Component")
	bool bTakeEnvironmentIntoAccount; //if not it does not matter if it's behind a wall, it will get damage

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Component")
	bool bHasFalloffExplosion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Component")
	float ExplosionMaxDamage; //Damage in explosion center
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Component")
	float ExplosionMinDamage; //Minimun damage when far from explosion center
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Component", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float ExplosionInnerRadius; //Radius of FullDamage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Component", meta = (ClampMin = "0.2", UIMin = "0.2"))
	float ExplosionOuterRadius; //It's the full explosion radius

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Component")
	float ExplosionDelay;

	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnExplodeSignature OnExplodeDelegate;

public:	
	// Sets default values for this component's properties
	UUB_ExplosionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Explosion Component")
	void Explode();

public:
	UFUNCTION(BlueprintCallable, Category = "Explosion Component")
	void StartExplosion(const TArray<AActor*> IgnoreActors);
};
