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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	float AimError; //+- error

	FVector LongRangeTargetPoint;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	bool bLoopPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	int DirectionIndexPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	float WaitingTimeOnPathPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Navigation Path")
	AUB_PathPoints* NavigationPath;

public:
	AUB_Enemy();

protected:
	UFUNCTION(BlueprintCallable)
	void SetLongRangeTargetPoint(AUB_Character* Target); //AI controller set this, later used in GetViewRotation

	virtual FRotator GetViewRotation() const override;

};
