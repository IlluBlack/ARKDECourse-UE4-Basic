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
class IUB_InteractiveItemInterface;
class AUB_Projectile;
class UUB_ProjectileTrajectoryComponent;

UENUM(Blueprintable)
enum class EUB_CharacterType : uint8
{
	CharacterType_Player	UMETA(displayName = "Player"),
	CharacterType_Enemy		UMETA(DisplayName = "Enemy"),
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUB_ProjectileTrajectoryComponent* DrawProjectiletTrajectoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ReferenceStartLocationThrow;

//Variables
private: //these ones are used just for internal logic
	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool bIsPressingSprint;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool bToggleSprintState;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool bIsPressingCrouchOrSlide;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EUB_CharacterType CharacterType;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UUB_CharacterInventory* Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocketName; //at this moment all weapons are placed in thee same socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AUB_Weapon> InitialWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw")
	TSubclassOf<AUB_Projectile> InitialThrowableItemClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throw")
	float ThrowSpeed;

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

	//Interact
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float MaxDistanceToInteract;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float TickInteractionFrequency;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Debug")
	bool bDebugInteractionLineTrace;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bIsInteracting;
	IUB_InteractiveItemInterface* AimingAtItem;
	FTimerHandle TimerHandle_Interaction;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	AUB_Weapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DenyAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = "0.0", UIMin = "0.0", ClamMax = "1.0", UIMax = "1.0"))
	float Accuracy; //TODO in the player this could change depending on velocity and if the player is crouching or not
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Body Targets")
	TArray<FName> BodyTargetsSocketName;

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

	void TickInteraction();
	void StartInteraction();
	void StopInteraction();
	bool IsAimingAtAnyInteractiveItem(IUB_InteractiveItemInterface*& InteractiveItem);

	void CreateInventory();

	void CreateInitialWeapon();
	void EquipWeapon(AUB_Weapon* Weapon);

	UFUNCTION(BlueprintCallable)
	void StartWeaponAction();
	UFUNCTION(BlueprintCallable)
	void StopWeaponAction();
	void StartAdditionalWeaponAction();
	UFUNCTION(BlueprintCallable)
	void StartWeaponPunchAction();
	void ChangeWeaponMode();

	void StartThrowAction();
	void StopThrowAction();
	void ThrowItem();

	UFUNCTION()
	virtual void OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Value, const UDamageType* DamageType, AController* InstigatedBy, AActor* ActorCauser);
	UFUNCTION()
	virtual void OnDead(AActor* ActorCauser);

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

	UFUNCTION(BlueprintCallable)
	EUB_CharacterType GetCharacterType() { return CharacterType; }

	virtual void LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride) override;

	//GetActorEyesViewPoint() is override for APawn and return in location GetPawnViewLocation() that is actually the eyes of the mesh player
	//override this function to make eyesViewPoint locate in the current camera
	virtual FVector GetPawnViewLocation() const override;

	//Inventory
	UFUNCTION(BlueprintCallable)
	UUB_CharacterInventory* GetInventory() const { return Inventory; };

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

	//Throw
	FVector GetStartLocationThrow() const;
	FVector GetThrowVelocity() const;

	//Ultimate
	void EarnUltimateXP(float XP);
	void OnFinishedUltimate();

	void SetSpeedModifier(float SpeedMod);
	void SetSpeedModifierToDefault();

	//Health
	UFUNCTION(BlueprintCallable)
	UUB_HealthComponent* GetHealthComponent() const { return HealthComponent; };
	bool TryGiveHealth(float ExtraHealth, AController* InstigatedBy = nullptr, AActor* ActorCauser = nullptr);

	//Dead
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;
	bool ShouldDissapearWhenDead() const { return bShouldDissapearWhenDead; };
};
