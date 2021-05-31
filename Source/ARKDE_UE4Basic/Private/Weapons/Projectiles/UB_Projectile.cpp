// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Projectile.h"
#include "UB_Character.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AUB_Projectile::AUB_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollider"));
	RootComponent = ProjectileCollider;

	ProjectileCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //overlap with pawns
	ProjectileCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(ProjectileCollider);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	//There's no need to attach this to the root, because this is not an USceneComponent
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 30000.0f;
}

// Called when the game starts or when spawned
void AUB_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUB_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUB_Projectile::SetCharacterOwner(AUB_Character* NewOwner)
{
	if (IsValid(NewOwner)) {
		SetOwner(NewOwner);
		CharacterOwner = NewOwner;
	}
}

float AUB_Projectile::GetRadius() const {
	return ProjectileCollider->GetScaledSphereRadius();
}

void AUB_Projectile::SetProjectileVelocity(FVector NewVelocity) {
	ProjectileMovementComponent->Velocity = NewVelocity;
}

