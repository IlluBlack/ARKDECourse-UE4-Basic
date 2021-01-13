// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UB_Character.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UUB_CharacterInventory;
class AUB_CharacterUltimate;
class UUB_HealthComponent;
class AUB_Weapon;
class AUB_Firearm;
class AUB_MeleeWeapon;
class UAnimMontage;
class UAnimInstance;
class AUB_GameMode;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUB_HealthComponent* HealthComponent;

//Variables
private: //these ones are used just for internal logic
	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool bIsPressingSprint;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool bToggleSprintState;
	UPROPERTY(VisibleAnywhere, Category = "Input")
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0", UIMin = "0.1"))
	float MaxRunSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float MaxCrouchSpeed; //the one setted in character movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0", UIMin = "0.1"))
	float MaxSlideSpeed;
	
	//TODO: Maybe fix this slider not with time but with velocity, if it's going down a mountain, they should be able to go down all sliding
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float MinSpeedToSlide;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float MinSlideDuration; //if speed is minSpeedToSlide what's the duration of sliding
	//UPROPERTY(BlueprintReadOnly, Category = "Movement")
	//float CurrentSlidingTime;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CurrentSlideDuration;
	FTimerHandle TimerHandle_Slide;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SpeedModifier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UUB_CharacterInventory> InventoryClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocketName; //at this moment all weapons are placed in thee same socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AUB_Weapon> InitialWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	TSubclassOf<AUB_CharacterUltimate> UltimateClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate")
	AUB_CharacterUltimate* CurrentUltimate;
	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	bool bCanUseUltimate;
	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	bool bIsUsingUltimate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate", meta = (ClampMin = "0.0", UIMin = "0.1"))
	float MaxUltimateXP;
	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	float CurrentUltimateXP;

	AUB_GameMode* GameModeReference;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Death")
	bool bShouldDissapearWhenDead;

	//Animation
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsFullBodyAnimation; //use as flag to know if animMontage is FULL_BODY or not
	FTimerHandle TimerHandle_FullBodyAnimation;

public:
	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool bIsPressingWeaponAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UUB_CharacterInventory* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	AUB_Weapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DenyAnimMontage;

//Functions
public:
	// Sets default values for this character's properties
	AUB_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitializeReferences();
	void SetupCharacterMovement();

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

	void ResolveMovement(); //**Define what movement apply**
	void ResolveMaxMovementSpeed(bool forceUpdate = false);
	void ResolveMaxCrouchMovementSpeed();

	void CreateInventory();

	void CreateInitialWeapon();
	void EquipWeapon(AUB_Weapon* Weapon);

	void StartWeaponAction();
	void StopWeaponAction();
	void StartAdditionalWeaponAction();
	void StartWeaponPunchAction();
	void ChangeWeaponMode();

	UFUNCTION()
	void OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void InitializeUltimate();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_EarnUltimateXP(float XP);
	void StartUltimate();
	void ResetUltimateXP();

	void ResetFullBodyAnimationFlag();

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

	//Animation
	float PlayAnimMontage(UAnimMontage* AnimMontage, bool bIsFullBody = false, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	void PlaySectionAnimMontage(FName Section, const UAnimMontage* Montage);

	//THESE FUNCTIONS CAN BE CALLED from Anim notifiers
	void OnFinishedWeaponAction();
	UFUNCTION(BlueprintCallable) //called from BP
	void OnFinishedAdditionalWeaponAction();
	void OnFinishedWeaponPunchAction();

	//MeleeCombos = These combo functions are going to be called from animNotify in eventGraph
	//(they could be made in an animationNotify class, but I want to have it in BP to practice in BP too)
	UFUNCTION(BlueprintCallable)
	void ANEnableMeleeCombo();
	UFUNCTION(BlueprintCallable)
	void ANResetMeleeCombo();

	//Ultimate
	void EarnUltimateXP(float XP);
	void OnFinishedUltimate();

	void SetSpeedModifier(float SpeedMod);
	void SetSpeedModifierToDefault();

	//Dead
	bool ShouldDissapearWhenDead() { return bShouldDissapearWhenDead; };
};
