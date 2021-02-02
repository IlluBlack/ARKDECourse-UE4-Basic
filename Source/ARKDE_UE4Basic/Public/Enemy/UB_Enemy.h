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
	bool bCanMeleeAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	float DistanceCloseWhenMelee;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	float MinDistanceToMeleeAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	bool bCanLongRangeAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	float DistanceCloseWhenLongRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	float MinDistanceToLongRangeAttack; //TODO: This should depend on the type of weapon, not the same to have a shotgun or a rifle...
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	float AimError; //+- error

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Navigation Path")
	AUB_PathPoints* navigationPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	bool bLoopPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	int DirectionIndexPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
	float WaitingTimeOnPathPoint;

	FVector LongRangeTargetPoint;

public:
	AUB_Enemy();

protected:
	UFUNCTION(BlueprintCallable)
	void SetLongRangeTargetPoint(AUB_Character* Target); //AI controller set this, later used in GetViewRotation

	virtual FRotator GetViewRotation() const override;

};
