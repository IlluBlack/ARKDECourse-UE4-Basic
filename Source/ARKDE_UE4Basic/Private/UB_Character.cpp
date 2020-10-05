// Fill out your copyright notice in the Description page of Project Settings.

#include "UB_Character.h"

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
}

// Called when the game starts or when spawned
void AUB_Character::BeginPlay()
{
	Super::BeginPlay();
	
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	//StandingCapsuleHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	ECurrentMovementState = EMovementState::Standing;

	bIsPressingSprint = false;
	bToggleSprintState = false;
	bIsPressingCrouchOrSlide = false;
	ResetMaxMovementSpeed();
}

// Called every frame
void AUB_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AUB_Character::StopCrouching);
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
	if (ECurrentMovementState == EMovementState::Crouching) //uncrouch and then jump
		StopCrouching();

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
	bIsPressingCrouchOrSlide = false;

	if (ECurrentMovementState == EMovementState::Crouching) { //Just when is actually crouching
		Super::UnCrouch(true);
		
		ECurrentMovementState = EMovementState::Standing;
		
		//GetCapsuleComponent()->SetCapsuleHalfHeight(StandingCapsuleHeight, true);
		ResolveMovement();
	}
}

//Slide
void AUB_Character::Slide()
{
	//Get current direction and slide in that direction
	//ECurrentMovementState = EMovementState::Sliding;
	
	//Verify if it's not going backwards, just slide when forward
	//FVector currentDirection = GetVelocity().GetSafeNormal();

	//GetActorForwardVector();

	//first crouch

	//then slide
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
					Slide();
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
