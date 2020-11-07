// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UB_Character.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UUB_CharacterInventory;
class AUB_Weapon;
class AUB_Firearm;
class AUB_MeleeWeapon;
class UAnimMontage;
class UAnimInstance;

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Standing,
	Falling,
	Crouching,
	Sliding,
};

UENUM(BlueprintType)
enum class EActionState : uint8 //UpperBody actions
{
	Default, //locomotion or other states that does not interfere
	Reloading,
	WeaponAction, //shooting...
	WeaponPunchAction, //punching with weapon
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
	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool bIsPressingSprint;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool bToggleSprintState;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool bIsPressingCrouchOrSlide;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool bIsPressingWeaponAction;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0", UIMin = "0.1"))
	float MaxRunSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float MaxCrouchSpeed; //the one setted in character movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0", UIMin = "0.1"))
	float MaxSlideSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0", UIMin = "0.1"))
	float MinDurationSlide; //if velocity is maxWalkSpeed what's the duration of sliding
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CurrentSlidingTime;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DurationSlide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UUB_CharacterInventory> InventoryClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocketName; //at this moment all weapons are placed in thee same socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AUB_Weapon> InitialWeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	EActionState ECurrentActionState;

	UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DenyAnimMontage; //look for an animation of player moving face like saying nope
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* ReloadWeaponAnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* WeaponPunchAnimMontage; //same weapon punch animation for all weapons
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<UAnimMontage*> MeleeWeaponAnimMontages; //all melee will same combo animations

	//Combos are just with meleeWeapons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combos")
	bool bIsMeleeComboEnabled; //combos are enabled just in some specific part of the animation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combos")
	int CurrentStepMeleeCombo;

	bool bIsCurrentWeaponMelee;
	AUB_Firearm* CurrentFirearm;
	AUB_MeleeWeapon* CurrentMeleeWeapon;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UUB_CharacterInventory* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	AUB_Weapon* CurrentWeapon;

//Functions
public:
	// Sets default values for this character's properties
	AUB_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitializeReferences();

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
	void ResolveMovement(); //**Define what movement apply**

	void CreateInventory();

	void CreateInitialWeapon();
	void EquipWeapon(AUB_Weapon* Weapon);

	void SetActionState(EActionState NewActionState);

	void StartWeaponAction(); //can be melee or firearm
	void StopWeaponAction();
	
	void VerifyAutomaticFirearm();

	void ChangeWeaponMode();

	void ReloadWeapon();
	void StopReloadingWeapon();

	void StartWeaponPunchAction(); //It does not mean meleeWeapon, it means weapon punch (golpe con el arma)

	void PlayAnimMontage(UAnimMontage* animMontage);

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

	void OnFinishedWeaponAction();
	UFUNCTION(BlueprintCallable)
	void OnFinishedReloadingWeapon(); //called from anim notify
	void OnFinishedWeaponPunchAction();

	//MeleeCombos = These combo functions are going to be called from animNotify in eventGraph
	UFUNCTION(BlueprintCallable)
	void SetMeleeComboEnabled(bool NewState);
	UFUNCTION(BlueprintCallable)
	void ResetMeleeCombo();

	AUB_MeleeWeapon* GetCurrentMeleeWeapon(); //for some animations to acces its collider
};
