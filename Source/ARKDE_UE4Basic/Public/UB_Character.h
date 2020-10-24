// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UB_Character.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UUB_CharacterInventory;
class AUB_Weapon;

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Standing,
	Falling,
	Crouching,
	Sliding,
};

UCLASS()
class ARKDE_UE4BASIC_API AUB_Character : public ACharacter
{
	GENERATED_BODY()

//Components
protected: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FPSCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* TPSSpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* TPSCameraComponent;

//Variables
private: //these ones are used just for internal logic
	UPROPERTY(VisibleAnywhere, Category = "Movement Input")
	bool bIsPressingSprint;
	UPROPERTY(VisibleAnywhere, Category = "Movement Input")
	bool bToggleSprintState;
	UPROPERTY(VisibleAnywhere, Category = "Movement Input")
	bool bIsPressingCrouchOrSlide;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	bool bUseFirstPersonView;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	bool bIsInvertedLook;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	FName FPSCameraSocketName;
	//UPROPERTY(BlueprintReadOnly, Category = "Camera")
	//float StandingCapsuleHeight; //the one setted in the BP

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EMovementState ECurrentMovementState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bUseHoldToSprint;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float MaxWalkSpeed; //the one setted in character movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0", UIMin = "0"))
	float MaxRunSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float MaxCrouchSpeed; //the one setted in character movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0", UIMin = "0"))
	float MaxSlideSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0", UIMin = "0"))
	float MinDurationSlide; //if velocity is maxWalkSpeed what's the duration of sliding
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CurrentSlidingTime;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DurationSlide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UUB_CharacterInventory> InventoryClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AUB_Weapon> InitialWeaponClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	AUB_Weapon* CurrentWeapon;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UUB_CharacterInventory* Inventory;

//Functions
public:
	// Sets default values for this character's properties
	AUB_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	virtual void AddControllerPitchInput(float value) override;

	virtual void Jump() override;
	virtual void StopJumping() override;

	void Sprint();
	void StopSprinting();

	virtual void Falling() override;
	virtual void Landed(const FHitResult& Hit) override;

	void CrouchOrSlide();
	void StopCrouchingOrSliding();

	void Crouch();
	void StopCrouching();

	void Slide();
	void StopSliding();

	void ResetMaxMovementSpeed();
	void ResolveMovement();

	void CreateInventory();

	void CreateInitialWeapon();
	void StartWeaponAction();
	void StopWeaponAction();

	void VerifyData();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride) override;

	//GetActorEyesViewPoint() is override for APawn and return in location GetPawnViewLocation() that is actually the eyes of the mesh player
	//override this function to make eyesViewPoint locate in the current camera
	virtual FVector GetPawnViewLocation() const override;
};
