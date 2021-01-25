// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_PathPoints.h"
#include "Components/BillboardComponent.h"

// Sets default values
AUB_PathPoints::AUB_PathPoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PathPointBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("PathPoint Billboard"));
	RootComponent = PathPointBillboardComponent;
}

// Called when the game starts or when spawned
void AUB_PathPoints::BeginPlay()
{
	Super::BeginPlay();
	
}

