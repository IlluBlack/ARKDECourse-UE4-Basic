// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_TriggerItem.generated.h"

class UBoxComponent;
class AUB_Character;

UCLASS()
class ARKDE_UE4BASIC_API AUB_TriggerItem : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MainColliderComponent;
	
public:	
	// Sets default values for this actor's properties
	AUB_TriggerItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void OnCharacterOverlap(AUB_Character* Character);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Trigger") //implementation in BP
	void BP_OnCharacterOverlap(AUB_Character* Character);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
