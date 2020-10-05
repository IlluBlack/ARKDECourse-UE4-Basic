// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_Platform.generated.h"

class UStaticMeshComponent;

UCLASS()
class ARKDE_UE4BASIC_API AUB_Platform : public AActor
{
	GENERATED_BODY()
	
//Components
public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* CustomRootComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
	UStaticMeshComponent* PlatformMeshComponent;

//Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool bIsGoingUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float MinHeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float MaxHeight;

//Functions
public:	
	// Sets default values for this actor's properties
	AUB_Platform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void Move();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
