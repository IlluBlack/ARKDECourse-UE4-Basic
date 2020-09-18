// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_Door.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class ARKDE_UE4BASIC_API AUB_Door : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUB_Door();

	//Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* CustomRootComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* DoorFrameComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* DoorComponent;
	UPROPERTY(EditAnywhere, Category = "Door")
		float OpenAngle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenDoor();
};
