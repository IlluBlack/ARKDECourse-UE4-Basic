// Fill out your copyright notice in the Description page of Project Settings.
#include "UB_Platform.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AUB_Platform::AUB_Platform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	PlatformMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMeshComponent->SetupAttachment(CustomRootComponent);

	//default values
	bIsGoingUp = true;
	Speed = 10.0f;
	MinHeight = 0.0f;
	MaxHeight = 50.0f;
}

// Called when the game starts or when spawned
void AUB_Platform::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUB_Platform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}

void AUB_Platform::Move()
{
	FVector upDirection = UKismetMathLibrary::GetUpVector(GetActorRotation());
	//FVector upDirection = GetActorUpVector(); ?
	if (!bIsGoingUp)
		upDirection *= -1;

	FVector CurrentVelocity = upDirection * Speed;
	FVector NewPosition = GetActorLocation() + CurrentVelocity;
	SetActorLocation(NewPosition);

	if ((bIsGoingUp && GetActorLocation().Z >= MaxHeight) || (!bIsGoingUp && GetActorLocation().Z <= MinHeight)) {
		bIsGoingUp = !bIsGoingUp;
	}
}

