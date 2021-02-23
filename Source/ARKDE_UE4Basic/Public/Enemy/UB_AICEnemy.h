// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UB_AICEnemy.generated.h"

class AUB_Enemy;

class UAIPerceptionComponent;
class UBehaviorTree;
class UBlackboardComponent;

/** Used to define each different type of AI
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_AICEnemy : public AAIController
{
	GENERATED_BODY()

protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UBlackboardComponent* BlackboardComponent;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	UBehaviorTree* EnemyBehaviorTree;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Controller")
	AUB_Enemy* ControlledEnemy;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Controller")
	bool bIsReceivingDamage;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception")
	const int IndexPerceptionSight = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception")
	int IndexPerceptionDamage;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Navigation")
	FName LoopPathKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Navigation")
	FName DirectionPathIndexKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Navigation")
	FName PathPointIndexKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Navigation")
	FName WaitTimeOnPathPointKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Perception")
	FName TargetLocationKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Perception")
	FName CanSeePlayerKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Perception")
	FName IsInvestigatingKey;

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

public:
	AUB_AICEnemy();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

public:
	void SetReceivingDamage(bool NewState) { bIsReceivingDamage = NewState; };

};
