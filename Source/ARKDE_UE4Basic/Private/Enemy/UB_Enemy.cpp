// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Enemy.h"
#include "UB_Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

AUB_Enemy::AUB_Enemy()
{
	Accuracy = 0.4f;
	AimError = 2.5f;

	bLoopPath = false;
	DirectionIndexPath = 1;
	WaitingTimeOnPathPoint = 1.0f;
}

void AUB_Enemy::SetLongRangeTargetPoint(AUB_Character* Target)
{
	if (IsValid(Target)) {
		if (Target->BodyTargetsSocketName.Num() <= 0) {
			LongRangeTargetPoint = Target->GetTargetLocation();
			return;
		}

		const int BodySocketIndex = FMath::RandRange(0, Target->BodyTargetsSocketName.Num()-1);
		const FName BodySocketName = Target->BodyTargetsSocketName[BodySocketIndex];

		const FVector BodySocketLocation = Target->GetMesh()->GetSocketLocation(BodySocketName);
		const FVector AimErrorLocation = FVector(
			FMath::RandRange(-AimError, +AimError),
			FMath::RandRange(-AimError, +AimError),
			FMath::RandRange(-AimError, +AimError)
		);
		LongRangeTargetPoint = BodySocketLocation + AimErrorLocation;

		//UE_LOG(LogTemp, Log, TEXT("- BodySocketLocation %s"), *BodySocketLocation.ToString());
		//UE_LOG(LogTemp, Log, TEXT("ShotPoint %s"), *LongRangeTargetPoint.ToString());
	}
}

FRotator AUB_Enemy::GetViewRotation() const
{
	if (LongRangeTargetPoint != FVector::ZeroVector) {
		return UKismetMathLibrary::FindLookAtRotation(GetPawnViewLocation(), LongRangeTargetPoint);
	}

	return Super::GetViewRotation();
}
