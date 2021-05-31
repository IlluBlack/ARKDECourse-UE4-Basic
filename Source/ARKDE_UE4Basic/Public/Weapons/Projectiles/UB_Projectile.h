// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent;

class AUB_Character;

UCLASS()
class ARKDE_UE4BASIC_API AUB_Projectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* ProjectileCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

protected:
	AUB_Character* CharacterOwner;

public:	
	// Sets default values for this actor's properties
	AUB_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCharacterOwner(AUB_Character* NewOwner);

	UFUNCTION(BlueprintCallable)
	float GetRadius() const;
	UFUNCTION(BlueprintCallable)
	void SetProjectileVelocity(FVector NewVelocity);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void Throw() {};
};
