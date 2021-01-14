// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_CollectableItem.generated.h"

class USphereComponent;
class AUB_Character;

UCLASS()
class ARKDE_UE4BASIC_API AUB_CollectableItem : public AActor
{
	GENERATED_BODY()

//Components
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* MainColliderComponent;

//Variables
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectable")
	float XPValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collectable")
	bool bShouldDestroyWhenPickedUp;

public:
	// Sets default values for this pawn's properties
	AUB_CollectableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void Pickup(AUB_Character* PickupCharacter);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Collectable") //implementation in BP
	void BP_Pickup(AUB_Character* PickupCharacter);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
