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
class UAnimMontage;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Punch")
	bool bIsPunching;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* PunchAnimMontage; //punch animation for this weapon

	AUB_Character* CurrentOwnerCharacter;
	
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

	//Punch
	UFUNCTION() //must have this if is gonna be added to OnComponentBeginOverlap
	virtual void ApplyPunchDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayAnimMontageInOwner(UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	void StopAnimMontageInOwner(UAnimMontage* AnimMontage);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EquipWeapon() {} //To reset variables inside the weapon or whatever needed

	UFUNCTION(BlueprintCallable) //Don't put this UFUNCTION in classes that are inheriting
	virtual void StartAction();
	UFUNCTION(BlueprintCallable)
	virtual void StopAction();

	UFUNCTION(BlueprintCallable)
	virtual void StartAdditionalAction();

	//Weapon punch
	UFUNCTION(BlueprintCallable)
	virtual void StartPunchAction();

	//Change Mode
	UFUNCTION(BlueprintCallable) //change to another mode if this weapon has one
	virtual void ChangeWeaponMode() {}

	UFUNCTION(BlueprintCallable)
	void SetCharacterOwner(AUB_Character* NewOwner);

	//Animation notifiers that are called from character or anim notifiers
	virtual void OnFinishedAction() {}
	virtual void OnFinishedAdditionalAction() {}
	virtual void OnFinishedPunchAction(); //finished the critic part of punch action

	virtual void EnablePunchDetector(); //EnableCollider
	virtual void DisablePunchDetector(); //EnableCollider
};
