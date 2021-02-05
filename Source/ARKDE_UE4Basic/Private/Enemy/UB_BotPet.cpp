// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_BotPet.h"
#include "UB_Character.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUB_BotPet::AUB_BotPet()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent")); //returns pointer
	//RootComponent = CustomRootComponent;

	BotMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BotMeshComponent"));
	RootComponent = BotMeshComponent;
	//BotMeshComponent->SetupAttachment(CustomRootComponent);
	BotMeshComponent->SetCanEverAffectNavigation(false);
	BotMeshComponent->SetSimulatePhysics(true);

	MinDistanceToTarget = 100.0f;
	ForceMagnitude = 1000.0f;
	ForceBoneName = NAME_None;
}

// Called when the game starts or when spawned
void AUB_BotPet::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn)) {
		PlayerCharacter = Cast<AUB_Character>(PlayerPawn);
	}

	NextPathPoint = GetNextPathPoint();
}

// Called every frame
void AUB_BotPet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Move to path point
	const float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
	if (DistanceToTarget <= MinDistanceToTarget) { //arrived to the target point
		NextPathPoint = GetNextPathPoint();
	}
	else {
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		const FVector Force = ForceDirection * ForceMagnitude;

		BotMeshComponent->AddForce(Force, ForceBoneName, true);
	}

	if (bDrawNextPathPoint) {
		DrawDebugSphere(GetWorld(), NextPathPoint, 30.0f, 15.0f, FColor::Purple, false, 0.0f, 0, 1.0f);
	}
}

FVector AUB_BotPet::GetNextPathPoint()
{
	if (IsValid(PlayerCharacter)) {
		UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), PlayerCharacter);
		if (NavigationPath->PathPoints.Num() > 1) { //0 is always the StartPosition
			return NavigationPath->PathPoints[1]; //nextPoint
		}
	}

	return GetActorLocation();
}

