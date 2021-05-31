// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_ProjectileTrajectoryComponent.h"
#include "UB_Character.h"

#include "Engine/Classes/Components/SplineComponent.h"
#include "Engine/Classes/Components/SplineMeshComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UUB_ProjectileTrajectoryComponent::UUB_ProjectileTrajectoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ProjectileSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("ProjectileSplineComponent"));

	//This is just the template to use
	//SplineMeshComponent = CreateDefaultSubobject<USplineMeshComponent>(TEXT("ProjectileSplineMeshComponent"));

	IndividualMeshLength = 100.0f;
}


// Called when the game starts
void UUB_ProjectileTrajectoryComponent::BeginPlay()
{
	Super::BeginPlay();

	//SplineMeshComponent->SetVisibility(false);
	CharacterOwner = Cast<AUB_Character>(GetOwner());

	if (IsValid(SplineEndPointMeshClass)) {
		SplineEndPointMesh = GetWorld()->SpawnActor<AActor>(SplineEndPointMeshClass);
		SetVisibilitySplineEndPointMesh(false);
	}
}

// Called every frame
void UUB_ProjectileTrajectoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bIsShowingPrediction) {

		FHitResult HitResult;
		FOccluderVertexArray PathPoints;
		FVector LastTraceDestination;

		FVector StartPosition;
		if (IsValid(CharacterOwner)) StartPosition = CharacterOwner->GetStartLocationThrow();
		else StartPosition = GetOwner()->GetActorLocation();

		FVector ThrowVelocity;
		if (IsValid(CharacterOwner)) {
			ThrowVelocity = CharacterOwner->GetThrowVelocity();
		}
		else {
			FVector ThrowDirection = GetOwner()->GetActorRotation().Vector();
			ThrowVelocity = ThrowDirection * ThrowSpeed;
		}

		//EDrawDebugTrace DebugTrace = bDebugTrajectory ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		bool bHit = UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), HitResult, PathPoints, LastTraceDestination, StartPosition, ThrowVelocity,
			true, ProjectileRadius, ECollisionChannel::ECC_Visibility, false, IgnoreActors, 
			bDebugTrajectory ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, 0.0f);

		if (bHit) {
			if (IsValid(SplineEndPointMesh)) {
				SplineEndPointMesh->SetActorLocation(HitResult.ImpactPoint);
			}
			SetVisibilitySplineEndPointMesh(true);
		}
		else {
			SetVisibilitySplineEndPointMesh(false);
		}

		ProjectileSplineComponent->SetSplinePoints(PathPoints, ESplineCoordinateSpace::World);
		DestroyPredictionMeshes();

		/*float SplineLength = ProjectileSplineComponent->GetSplineLength();
		int NumMeshes = SplineLength / IndividualMeshLength;

		for (int Index = 0; Index < NumMeshes; Index++)
		{
			USplineMeshComponent* Mesh = NewObject<USplineMeshComponent>(this, SplineMeshClass);
			//Mesh->ForwardAxis = ESplineMeshAxis::Z;

			FVector LocationAlongSpline;
			FVector TangentAlongSpline;
			GetLocationAndTangentAlongSplineAtIndex(Index, LocationAlongSpline, TangentAlongSpline);
			FVector NextLocationAlongSpline;
			FVector NextTangentAlongSpline;
			GetLocationAndTangentAlongSplineAtIndex(Index + 1, NextLocationAlongSpline, NextTangentAlongSpline);

			Mesh->SetStartAndEnd(LocationAlongSpline, TangentAlongSpline, NextLocationAlongSpline, NextTangentAlongSpline);

			ProjectileSplineMeshes.Add(Mesh);
		}*/
	}
}

void UUB_ProjectileTrajectoryComponent::GetLocationAndTangentAlongSplineAtIndex(int Index, FVector& LocationAlongSpline, FVector& TangentAlongSpline) const
{
	const float DistanceAlongSpline = Index * IndividualMeshLength;
	LocationAlongSpline = ProjectileSplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	TangentAlongSpline = ProjectileSplineComponent->GetTangentAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	//Clamp TangentSize for it to not be longer than IndividualMeshLength
	TangentAlongSpline = TangentAlongSpline.GetClampedToSize(0.0f, IndividualMeshLength);
}

void UUB_ProjectileTrajectoryComponent::ShowTrajectoryPrediction(float _ProjectileSpeed, float _ProjectileRadius, TArray<AActor*> _IgnoreActors)
{
	ThrowSpeed = _ProjectileSpeed;
	ProjectileRadius = _ProjectileRadius;
	IgnoreActors = _IgnoreActors;

	bIsShowingPrediction = true;
}

void UUB_ProjectileTrajectoryComponent::HideTrajectoryPrediction()
{
	bIsShowingPrediction = false;

	DestroyPredictionMeshes();
	SetVisibilitySplineEndPointMesh(false);
}

void UUB_ProjectileTrajectoryComponent::SetVisibilitySplineEndPointMesh(bool bNewState)
{
	if (IsValid(SplineEndPointMesh)) {
		SplineEndPointMesh->SetActorHiddenInGame(!bNewState);
	}
}

void UUB_ProjectileTrajectoryComponent::DestroyPredictionMeshes()
{
	for (USplineMeshComponent* SplineMesh : ProjectileSplineMeshes) {
		SplineMesh->DestroyComponent();
	}

	ProjectileSplineMeshes.Empty();
}
