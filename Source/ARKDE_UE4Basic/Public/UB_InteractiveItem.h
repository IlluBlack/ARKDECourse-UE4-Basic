// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractiveItems/UB_InteractiveItemInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_InteractiveItem.generated.h"

UCLASS()
class ARKDE_UE4BASIC_API AUB_InteractiveItem : public AActor, public IUB_InteractiveItemInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bIsInteracting;
	//AActor* InteractingWith;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float HoldTimeToFireInteraction;
	FTimerHandle TimerHandle_HoldInteraction;
	FTimerDelegate TimerDelegate_HoldInteraction;
	
public:	
	// Sets default values for this actor's properties
	AUB_InteractiveItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual bool CanStartInteraction(AActor* ActorInteracting);
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void FireInteraction(AActor* ActorInteracting);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void BP_FireInteraction(AActor* ActorInteracting);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//startfocus
	//endfocus

	virtual void StartInteraction(AActor* ActorInteracting) override;
	virtual void StopInteraction(AActor* ActorInteracting) override;

};
