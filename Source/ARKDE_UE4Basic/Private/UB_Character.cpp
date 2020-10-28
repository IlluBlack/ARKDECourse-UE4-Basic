// Fill out your copyright notice in the Description page of Project Settings.

#include "UB_Character.h"
#include "UB_CharacterInventory.h"
#include "Weapons/UB_Weapon.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AUB_Character::AUB_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseFirstPersonView = true;
	bIsInvertedLook = false;

	FPSCameraSocketName = "SCK_FPSCamera";
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true; 
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName); //Needs to be son of the mesh to be able to be in the socket

	TPSSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent")); //to make TPS camera rotate with player as pivot
	TPSSpringArmComponent->bUsePawnControlRotation = true;
	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->SetupAttachment(TPSSpringArmComponent);

	bUseHoldToSprint = true;
	MaxRunSpeed = 800;
	MaxSlideSpeed = 1000;
	MinDurationSlide = 1.0f;

	WeaponSocketName = "SCK_Weapon";
}

// Called when the game starts or when spawned
void AUB_Character::BeginPlay()
{
	Super::BeginPlay();

	//Inventory = NewObject<UUB_CharacterInventory>(); //not now, using TSubclassOf
	CreateInventory();
	CreateInitialWeapon();
	
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	MaxCrouchSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;
	//StandingCapsuleHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	ECurrentMovementState = EMovementState::Standing;

	bIsPressingSprint = false;
	bToggleSprintState = false;
	bIsPressingCrouchOrSlide = false;
	ResetMaxMovementSpeed();

	//Always at the end
	VerifyData();
}

// Called every frame
void AUB_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Counter to finish sliding
	if (ECurrentMovementState == EMovementState::Sliding) {
		CurrentSlidingTime += DeltaTime;

		if (CurrentSlidingTime >= DurationSlide) {
			StopSliding();
		}
	}

}

//ViewLocation is going to be the camera position
FVector AUB_Character::GetPawnViewLocation() const
{
	if (bUseFirstPersonView) {
		if (IsValid(FPSCameraComponent)) {
			return FPSCameraComponent->GetComponentLocation();
		}
	}
	else {
		if (IsValid(TPSCameraComponent)) {
			return TPSCameraComponent->GetComponentLocation();
		}
	}

	return Super::GetPawnViewLocation();
}

// Called to bind functionality to input
void AUB_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AUB_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUB_Character::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AUB_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ACharacter::AddControllerYawInput); //directly the one from ACharacter and APawn class

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUB_Character::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AUB_Character::StopJumping); //not mandatory to call

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AUB_Character::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AUB_Character::StopSprinting);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AUB_Character::CrouchOrSlide);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AUB_Character::StopCrouchingOrSliding);

	PlayerInputComponent->BindAction("WeaponAction", IE_Pressed, this, &AUB_Character::StartWeaponAction);
	PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &AUB_Character::StopWeaponAction);

	PlayerInputComponent->BindAction("ChangeWeaponMode", IE_Pressed, this, &AUB_Character::ChangeWeaponMode);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AUB_Character::ReloadWeapon);
}

//Move
void AUB_Character::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}
void AUB_Character::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

//Look
void AUB_Character::AddControllerPitchInput(float value)
{
	Super::AddControllerPitchInput(bIsInvertedLook ? -value : value);
}

//Jump
void AUB_Character::Jump()
{
	StopCrouchingOrSliding(); //uncrouch and then jump

	Super::Jump();
}
void AUB_Character::StopJumping()
{
	Super::StopJumping();
}

//Sprint
void AUB_Character::Sprint()
{
	bIsPressingSprint = true;
	bToggleSprintState = !bToggleSprintState;

	ResolveMovement();
}
void AUB_Character::StopSprinting()
{
	bIsPressingSprint = false;

	if (bUseHoldToSprint) {
		ResolveMovement();
	}
}

//Fall
void AUB_Character::Falling()
{
	StopCrouchingOrSliding(); //stop this to return collider to its size

	Super::Falling();

	ECurrentMovementState = EMovementState::Falling;
}
void AUB_Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	ECurrentMovementState = EMovementState::Standing;
	ResolveMovement();
}

void AUB_Character::CrouchOrSlide()
{
	bIsPressingCrouchOrSlide = true;
	ResolveMovement();
}

void AUB_Character::StopCrouchingOrSliding()
{
	bIsPressingCrouchOrSlide = false;

	if (ECurrentMovementState == EMovementState::Crouching)
		StopCrouching();
	else if (ECurrentMovementState == EMovementState::Sliding)
		StopSliding();
}

//Crouch
void AUB_Character::Crouch()
{
	Super::Crouch(true);

	ECurrentMovementState = EMovementState::Crouching;
	//Manual crouch, because the crouch function moves FPS Camera
	//GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchedEyeHeight, true);
}
void AUB_Character::StopCrouching()
{
	Super::UnCrouch(true);
		
	ECurrentMovementState = EMovementState::Standing;
	//GetCapsuleComponent()->SetCapsuleHalfHeight(StandingCapsuleHeight, true);
	ResolveMovement();
}

//Slide
void AUB_Character::Slide()
{
	//TODO:
	//Verify if it's not going backwards, just slide when forward
	//FVector currentDirection = GetVelocity().GetSafeNormal();
	//GetActorForwardVector();

	//First crouch
	Crouch();
	//as this crouch is controlled by character movement change max crouch speed
	GetCharacterMovement()->MaxWalkSpeedCrouched = MaxSlideSpeed;

	//then slide
	float currentSpeed = GetVelocity().Size(); //magnitude
	DurationSlide = (currentSpeed*MinDurationSlide)/MaxWalkSpeed;
	CurrentSlidingTime = 0;

	ECurrentMovementState = EMovementState::Sliding;
}

void AUB_Character::StopSliding()
{
	bIsPressingCrouchOrSlide = false; //Not allow player to continue sliding, if the time was over
	StopCrouching(); //Slide is actually collide as crouching then stop crouching

	GetCharacterMovement()->MaxWalkSpeedCrouched = MaxCrouchSpeed; //return crouch speed to its default value
}

//Movement
void AUB_Character::ResolveMovement() //Reset, if user pressed keys or was pressing
{
	if (ECurrentMovementState == EMovementState::Standing) {
		if (bIsPressingCrouchOrSlide) {
			if (bUseHoldToSprint) {
				if (bIsPressingSprint) {
					Slide();
				}
				else {
					Crouch();
				}
			}
			else {
				if (bToggleSprintState) {
					if (GetVelocity().Size() > MaxWalkSpeed) //if it's actually running
						Slide();
					else
						Crouch();
				}
				else {
					Crouch();
				}
			}
		}
	}

	ResetMaxMovementSpeed();
}
void AUB_Character::ResetMaxMovementSpeed()
{
	bool updateSpeed = false;
	float currentMaxSpeed;
	//Update manually velocity if I'm controlling crouch by myself
	/*if (ECurrentMovementState == EMovementState::Crouching) {
		currentMaxSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;
		updateSpeed = true;
	}
	else */
	if (ECurrentMovementState == EMovementState::Standing) {
		if (bUseHoldToSprint) {
			currentMaxSpeed = bIsPressingSprint ? MaxRunSpeed : MaxWalkSpeed;
		}
		else {
			currentMaxSpeed = bToggleSprintState ? MaxRunSpeed : MaxWalkSpeed;
		}

		updateSpeed = true;
	}

	if (updateSpeed) {
		GetCharacterMovement()->MaxWalkSpeed = currentMaxSpeed;
	}
}

//Launch
void AUB_Character::LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	//Maybe add here some specific thing if I need it
	Super::LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}

//Inventory
void AUB_Character::CreateInventory()
{
	if (IsValid(InventoryClass)) {
		Inventory = InventoryClass->GetDefaultObject<UUB_CharacterInventory>();
	}
}

//Weapon
void AUB_Character::CreateInitialWeapon()
{
	if (IsValid(InitialWeaponClass)) {
		//TODO: Maybe spawn in a socket ?
		//CurrentWeapon = GetWorld()->SpawnActor<AUB_Weapon>(InitialWeaponClass, GetActorLocation(), GetActorRotation());
		CurrentWeapon = GetWorld()->SpawnActor<AUB_Weapon>(InitialWeaponClass);
		
		if (IsValid(CurrentWeapon)) {
			CurrentWeapon->SetCharacterOwner(this);
			//CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		}
	}
}
void AUB_Character::StartWeaponAction()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StartAction();
	}
}
void AUB_Character::StopWeaponAction()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StopAction();
	}
}
void AUB_Character::ChangeWeaponMode()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->ChangeWeaponMode();
	}
}
void AUB_Character::ReloadWeapon()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->Reload();
	}
}

//Data
void AUB_Character::VerifyData()
{
	if (!IsValid(CurrentWeapon)) UE_LOG(LogTemp, Warning, TEXT("Current weapon was not defined"));
	if (!IsValid(Inventory)) UE_LOG(LogTemp, Error, TEXT("Character inventory was not created correctly"));

	if (MaxWalkSpeed <= 0) UE_LOG(LogTemp, Error, TEXT("MaxWalkSpeed setted in CharacterMovementComponent is zero or less than zero"));
	if (MaxCrouchSpeed <= 0) UE_LOG(LogTemp, Error, TEXT("MaxCrouchSpeed setted in CharacterMovementComponent is zero or less than zero"));
}
