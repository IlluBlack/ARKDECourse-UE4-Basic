// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractiveItems/UB_InteractiveItemInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_ActivateBotsPlatform.generated.h"

class AUB_BotPetControlBase;
class AUB_Character;

UCLASS()
class ARKDE_UE4BASIC_API AUB_ActivateBotsPlatform : public AActor, public IUB_InteractiveItemInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activate Platform")
	TArray<AUB_BotPetControlBase*> BotControlBases;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activate Platform")
	bool bIsPlatformActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deactivate Platform")
	FName KeyTagToDeactivate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deactivate Platform")
	float HoldTimeToDeactivate;

public:	
	// Sets default values for this actor's properties
	AUB_ActivateBotsPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ActivatePlatform();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Activate Platform")
	void BP_ActivatePlatform();

	void DeactivatePlatform(bool bForceDeactivation = false);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Deactivate Platform")
	void BP_DeactivatePlatform();
	
	UFUNCTION()
	void EndHoldInteraction(AUB_Character* CharacterInteracting);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartInteraction(AActor* ActorInteracting) override;
	virtual void StopInteraction(AActor* ActorInteracting) override;

};
