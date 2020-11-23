// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_SpectatingCamera.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AUB_SpectatingCamera::AUB_SpectatingCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CameraMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMeshComponent"));
	RootComponent = CameraMeshComponent;
}

