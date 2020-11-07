// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UB_Weapon.generated.h"

//class ACharacter; //My character needs to be AUB_Character
class AUB_Character;
class UDamageType;
class USceneComponent;
class UCapsuleComponent;

UCLASS()
class ARKDE_UE4BASIC_API AUB_Weapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* CustomRootComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* PunchDetectorComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	//Each weapon can be used for punch, even if it's not a melee weapon, it's damage is lower
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Punch")
	float PunchDamage;

	ACharacter* CurrentOwnerCharacter;
	
public:	
	// Sets default values for this actor's properties
	AUB_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Primary action, fire...
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon")
	void BP_StartAction();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon")
	void BP_StopAction();

	//Secondary action, could be zoom

	//Additional action, reload...
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon")
	void BP_StartAdditionalAction();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon")
	void BP_StopAdditionalAction();

	UFUNCTION()
	virtual void ApplyPunchDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) //Don't put this UFUNCTION in classes that are inheriting
	virtual void StartAction();
	UFUNCTION(BlueprintCallable)
	virtual void StopAction();

	UFUNCTION(BlueprintCallable)
	virtual void StartAdditionalAction();
	UFUNCTION(BlueprintCallable)
	virtual void StopAdditionalAction();

	//If I want to make something specific when weapon punch
	UFUNCTION(BlueprintCallable)
	virtual void StartPunchAction() {};
	void SetPunchDetectorEnabled(ECollisionEnabled::Type NewCollisionState);

	UFUNCTION(BlueprintCallable)
	void SetCharacterOwner(AUB_Character* NewOwner);
	
};
