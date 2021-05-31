// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UB_ProjectileTrajectoryComponent.generated.h"

class USplineComponent;
class USplineMeshComponent;

class AUB_Character;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARKDE_UE4BASIC_API UUB_ProjectileTrajectoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* ProjectileSplineComponent;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	//USplineMeshComponent* SplineMeshComponent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Spline Mesh Trajectory")
	TArray<USplineMeshComponent*> ProjectileSplineMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline Mesh Trajectory")
	TSubclassOf<USplineMeshComponent> SplineMeshClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Mesh Trajectory")
	TSubclassOf<AActor> SplineEndPointMeshClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline Mesh Trajectory")
	float IndividualMeshLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline Trajectory")
	bool bDebugTrajectory;

	AActor* SplineEndPointMesh;

	bool bIsShowingPrediction;

	AUB_Character* CharacterOwner;
	float ThrowSpeed;
	float ProjectileRadius;
	TArray<AActor*> IgnoreActors;

public:	
	// Sets default values for this component's properties
	UUB_ProjectileTrajectoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void GetLocationAndTangentAlongSplineAtIndex(int Index, FVector& LocationAlongSpline, FVector& TangentAlongSpline) const;

	void SetVisibilitySplineEndPointMesh(bool bNewState);
	void DestroyPredictionMeshes();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ShowTrajectoryPrediction(float _ProjectileSpeed, float _ProjectileRadius, TArray<AActor*> _IgnoreActors = TArray<AActor*>{});
	void HideTrajectoryPrediction();
};
