// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_Weapon.generated.h"

class ACharacter;
class UDamageType;

UCLASS()
class ARKDE_UE4BASIC_API AUB_Weapon : public AActor
{
	GENERATED_BODY()

protected:
	//All weapons in this projects are skeletalMesh, then I am not using UStaticMesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMeshComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	ACharacter* CurrentOwnerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzzle")
	FName MuzzleSocketName;
	
public:	
	// Sets default values for this actor's properties
	AUB_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon")
	void BP_StartAction();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon")
	void BP_StopAction();

	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FVector GetMuzzleSocketLocation();
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FRotator GetMuzzleSocketRotation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) //Don't put this UFUNCTION in classes that are inheriting
	virtual void StartAction();
	UFUNCTION(BlueprintCallable)
	virtual void StopAction();
	UFUNCTION(BlueprintCallable)
	void SetCharacterOwner(ACharacter* NewOwner);

};
