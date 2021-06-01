// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/UB_BotPet.h"
#include "UB_BotPetHealer.generated.h"

class USphereComponent;
class AUB_Character;
class AUB_BotPetControlBase;
class UParticleSystemComponent;

UENUM(Blueprintable)
enum class EUB_BotPetHealerState : uint8
{
	BotState_Idle				UMETA(displayName = "Idle"),
	BotState_GoingToBase		UMETA(displayName = "Going to Base"),
	BotState_FollowingTarget	UMETA(displayName = "Following Target"),
	BotState_Healing			UMETA(displayName = "Healing"),
};

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bot Pet")
	EUB_BotPetHealerState CurrentState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Regeneration")
	float MaxDistanceOutOfControlBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing Effect")
	FName InternalRadiusParticlesParameterName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing Effect")
	float InternalRadiusParticlesBaseUnit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing Effect")
	FName ExternalRadiusParticlesParameterName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing Effect")
	float ExternalRadiusParticlesBaseUnit;
	
public:
	AUB_BotPetHealer();

protected:
	virtual void BeginPlay() override;

	bool IsCharacterValidAsTarget(AUB_Character* Character);

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
