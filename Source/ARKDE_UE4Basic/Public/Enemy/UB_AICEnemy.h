// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UB_AICEnemy.generated.h"

class UAIPerceptionComponent;
class AUB_Enemy;

/** Used to define each different type of AI
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_AICEnemy : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPerceptionComponent* AIPerceptionComponent;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Navigation")
	FName LoopPathKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Navigation")
	FName DirectionPathIndexKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Navigation")
	FName PathPointIndexKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Navigation")
	FName WaitTimeOnPathPointKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Perception")
	FName CanSeePlayerKey;

	//This variables are here and not in UB_Enemy because depend on AIPerception
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Melee")
	bool bHasMeleeEnabled;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Melee")
	float DistanceCloseWhenMelee;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Melee")
	float MinDistanceToMelee;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Long Range")
	bool bHasLongRangeEnabled;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Long Range")
	float DistanceCloseWhenLongRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Long Range")
	float MinDistanceToLongRange; //TODO: This could depend on the type of weapon, not the same to have a shotgun or a rifle...

	UPROPERTY(BlueprintReadWrite)
	AUB_Enemy* ControlledEnemy;

public:
	AUB_AICEnemy();

protected:
	virtual void BeginPlay() override;

};
