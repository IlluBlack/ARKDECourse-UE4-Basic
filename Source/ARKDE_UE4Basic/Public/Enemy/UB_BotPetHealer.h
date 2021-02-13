// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/UB_BotPet.h"
#include "UB_BotPetHealer.generated.h"

class USphereComponent;
class AUB_Character;
class AUB_BotPetControlBase;
class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_BotPetHealer : public AUB_BotPet
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* HealthRegenerationAreaComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* HealingParticlesComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Regeneration")
	float HealthRegenerationFrequency;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Regeneration")
	float HealthRegenerationRate;

	TArray<AUB_Character*> CharactersInRegenerationArea;
	FTimerHandle TimerHandle_HealthRegeneration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health Regeneration")
	bool bIsRegeneratingHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Base")
	AUB_BotPetControlBase* ControlBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Regeneration")
	float MaxDistanceOutOfControlBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealingParticles")
	FName InternalRadiusParticlesParameterName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealingParticles")
	float InternalRadiusParticlesBaseUnit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealingParticles")
	FName ExternalRadiusParticlesParameterName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealingParticles")
	float ExternalRadiusParticlesBaseUnit;
	
public:
	AUB_BotPetHealer();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOtherActorEnterRegenerationArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOtherActorExitRegenerationArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void VerifyHealthRegenerationTimer();
	void TickHealthRegeneration();

	virtual FVector GetNextPathPoint() override;
	AUB_Character* GetNextCharacterNeedingHealth();

public:
	virtual void Tick(float DeltaTime) override;

};
