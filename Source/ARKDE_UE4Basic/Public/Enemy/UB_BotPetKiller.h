// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/UB_BotPet.h"
#include "UB_BotPetKiller.generated.h"

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_BotPetKiller : public AUB_BotPet
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SelfDestructionDetectorComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Self Destruction")
	bool bStartedSelfDestructionCountdown;
	FTimerHandle TimerHandle_SelfDestructionCountdown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Self Destruction")
	float SelfDestructionCountdownFrequency;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Self Destruction")
	float SelfDestructionCountdownDamage;

public:
	AUB_BotPetKiller();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOtherActorsDetection(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SelfDamage();


public:
	virtual void Tick(float DeltaTime) override;

};
