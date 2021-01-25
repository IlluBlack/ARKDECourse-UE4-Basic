// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UB_Character.h"
#include "UB_Enemy.generated.h"

class AUB_PathPoints;

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_Enemy : public AUB_Character
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Navigation Path")
	AUB_PathPoints* navigationPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	bool bLoopPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	int DirectionIndexPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	float WaitingTimeOnPathPoint;

public:
	AUB_Enemy();
};
