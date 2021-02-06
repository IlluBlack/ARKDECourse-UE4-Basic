// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_AICEnemy.h"
#include "UB_Enemy.h"
#include "UB_PathPoints.h"
#include "Kismet/GameplayStatics.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"

AUB_AICEnemy::AUB_AICEnemy()
{
	LoopPathKey = "bLoopPath";
	DirectionPathIndexKey = "DirectionPathIndex";
	PathPointIndexKey = "PathPointIndex";
	WaitTimeOnPathPointKey = "WaitTimeOnPathPoint";

	CanSeePlayerKey = "bCanSeePlayer";

	bHasMeleeEnabled = true;
	DistanceCloseWhenMelee = 180.0f;
	MinDistanceToMelee = 300.0f;
	bHasLongRangeEnabled = true;
	DistanceCloseWhenLongRange = 950.0f;
	MinDistanceToLongRange = 1200.0f;
}

void AUB_AICEnemy::BeginPlay()
{
	Super::BeginPlay();

	APawn* ControlledPawn = K2_GetPawn();
	if (IsValid(ControlledPawn)) {
		AUB_Enemy* ControlledEnemy = Cast<AUB_Enemy>(ControlledPawn);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ControlledPawn is NULL"));
	}
	
	if (IsValid(ControlledEnemy)) {
		UBlackboardComponent* Blackboard = GetBlackboardComponent();

		if (IsValid(Blackboard)) {

			Blackboard->SetValueAsBool(LoopPathKey, ControlledEnemy->bLoopPath);
			Blackboard->SetValueAsInt(DirectionPathIndexKey, ControlledEnemy->DirectionIndexPath);
			int StartPathPointIndex = 0;
			if (ControlledEnemy->DirectionIndexPath < 0) { //go backwards
				StartPathPointIndex = ControlledEnemy->NavigationPath->PathPointsLength()-1;
			}
			Blackboard->SetValueAsInt(PathPointIndexKey, StartPathPointIndex);
			Blackboard->SetValueAsFloat(WaitTimeOnPathPointKey, ControlledEnemy->WaitingTimeOnPathPoint);

			//...
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ControlledPawn is not a AUB_Enemy"));
	}

}
