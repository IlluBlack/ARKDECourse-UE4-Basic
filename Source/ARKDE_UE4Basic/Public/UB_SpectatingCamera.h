// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_SpectatingCamera.generated.h"

class UStaticMeshComponent;

UENUM(Blueprintable)
enum class EUB_SpectatingCameraType : uint8
{
	CameraType_None		UMETA(displayName = "None"),
	CameraType_Victory	UMETA(DisplayName = "Victory"),
	CameraType_GameOver	UMETA(DisplayName = "Game Over"),
};

UCLASS()
class ARKDE_UE4BASIC_API AUB_SpectatingCamera : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CameraMeshComponent;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spectating Camera")
	EUB_SpectatingCameraType CameraType;

public:	
	// Sets default values for this actor's properties
	AUB_SpectatingCamera();

public:
	EUB_SpectatingCameraType GetCameraType() { return CameraType; };

};
