// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_AICEnemy.h"
#include "UB_Enemy.h"
#include "UB_PathPoints.h"
#include "UB_Character.h"

#include "Kismet/GameplayStatics.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "AIModule/Classes/Perception/AIPerceptionComponent.h"

AUB_AICEnemy::AUB_AICEnemy()
{
	//AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	//IndexPerceptionSight = 0;
	//IndexPerceptionDamage = 1;

	LoopPathKey = "bLoopPath";
	DirectionPathIndexKey = "DirectionPathIndex";
	PathPointIndexKey = "PathPointIndex";
	WaitTimeOnPathPointKey = "WaitTimeOnPathPoint";
	TargetLocationKey = "TargetLocation";

	CanSeePlayerKey = "bCanSeePlayer";
	IsInvestigatingKey = "bIsInvestigating";

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

	//AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AUB_AICEnemy::OnPerceptionUpdated);

	if (IsValid(EnemyBehaviorTree)) {
		RunBehaviorTree(EnemyBehaviorTree);
	}

	AUB_Enemy* ControlledEnemy = Cast<AUB_Enemy>(K2_GetPawn());
	
	if (IsValid(ControlledEnemy)) {
		BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(this);

		if (IsValid(BlackboardComponent)) {

			BlackboardComponent->SetValueAsBool(LoopPathKey, ControlledEnemy->bLoopPath);
			BlackboardComponent->SetValueAsInt(DirectionPathIndexKey, ControlledEnemy->DirectionIndexPath);
			int StartPathPointIndex = 0;
			if (ControlledEnemy->DirectionIndexPath < 0) { //go backwards
				StartPathPointIndex = ControlledEnemy->NavigationPath->PathPointsLength()-1;
			}
			BlackboardComponent->SetValueAsInt(PathPointIndexKey, StartPathPointIndex);
			BlackboardComponent->SetValueAsFloat(WaitTimeOnPathPointKey, ControlledEnemy->WaitingTimeOnPathPoint);

			//...
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ControlledEnemy is not VALID"));
	}

}

void AUB_AICEnemy::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	return;
	/*if (!IsValid(BlackboardComponent)) return;

	for (AActor* SensedActor : UpdatedActors) {
		if (IsValid(SensedActor)) {
			AUB_Character* SensedCharacter = Cast<AUB_Character>(SensedActor);

			if (IsValid(SensedCharacter) && SensedCharacter->GetCharacterType() == EUB_CharacterType::CharacterType_Player) {
				FActorPerceptionBlueprintInfo PerceptionInfo;
				AIPerceptionComponent->GetActorsPerception(SensedActor, PerceptionInfo);

				//Stimuled senses
				for(int idxSense = 0; idxSense < PerceptionInfo.LastSensedStimuli.Num(); idxSense++)
				{
					const int IndexPerceptionSight = 0;
					const int IndexPerceptionDamage = 1;

					FAIStimulus Stimuli = PerceptionInfo.LastSensedStimuli[idxSense];
					switch (idxSense)
					{
					case IndexPerceptionSight:
						BlackboardComponent->SetValueAsBool(CanSeePlayerKey, Stimuli.WasSuccessfullySensed());
						break;

					case IndexPerceptionDamage:
						if (bIsReceivingDamage) {
							BlackboardComponent->SetValueAsBool(IsInvestigatingKey, bIsReceivingDamage); //becomes false in BT
							bIsReceivingDamage = false;
							BlackboardComponent->SetValueAsVector(TargetLocationKey, Stimuli.StimulusLocation);
						}
						break;

					default:
						break;
					}
				}
			}
		}
	}*/
}
