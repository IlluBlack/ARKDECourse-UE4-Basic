// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_PathPoints.generated.h"

class UBillboardComponent;

UCLASS()
class ARKDE_UE4BASIC_API AUB_PathPoints : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* PathPointBillboardComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Path", meta = (MakeEditWidget = true))
	TArray<FVector> PathPoints;
	
public:	
	// Sets default values for this actor's properties
	AUB_PathPoints();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	int PathPointsLength() const { return PathPoints.Num(); }

};
