// Fill out your copyright notice in the Description page of Project Settings.

#include "UB_Character.h"
#include "UB_CharacterInventory.h"
#include "UB_Weapon.h"
#include "UB_MeleeWeapon.h"
#include "UB_CharacterUltimate.h"
#include "ARKDE_UE4Basic.h"
#include "InteractiveItems/UB_InteractiveItemInterface.h"
#include "Components/UB_HealthComponent.h"
#include "Components/UB_ProjectileTrajectoryComponent.h"
#include "Weapons/Projectiles/UB_projectile.h"
#include "Core/UB_GameInstance.h"
#include "Core/UB_GameMode.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUB_Character::AUB_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseFirstPersonView = true;
	bIsInvertedLook = false;

	FPSCameraSocketName = "SCK_FPSCamera"; //define first the socket, then attach
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true; 
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName); //Needs to be son of the mesh to be able to be in the socket

	TPSSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent")); //to make TPS camera rotate with player as pivot
	TPSSpringArmComponent->bUsePawnControlRotation = true;
	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->SetupAttachment(TPSSpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UUB_HealthComponent>(TEXT("HealthComponent"));

	DrawProjectiletTrajectoryComponent = CreateDefaultSubobject<UUB_ProjectileTrajectoryComponent>(TEXT("ProjectileTrajectoryComponent"));
	ReferenceStartLocationThrow = CreateDefaultSubobject<USceneComponent>(TEXT("ReferenceStartLocationThrow"));

	bUseHoldToSprint = true;
	MaxRunSpeed = 800;
	MaxSlideSpeed = 1200;
	MinSpeedToSlide = 820;

	MaxUltimateXP = 100.0f;

	WeaponSocketName = "SCK_Weapon";
	bIsFullBodyAnimation = false;

	ThrowSpeed = 2000.0f;

	Accuracy = 1.0f;

	MaxDistanceToInteract = 20.0f;
	TickInteractionFrequency = 0.2f;
}

// Called when the game starts or when spawned
void AUB_Character::BeginPlay()
{
	Super::BeginPlay();

	ResetFullBodyAnimationFlag();
	InitializeReferences();
	CreateInventory();
	CreateInitialWeapon();
	InitializeUltimate();
	
	SetupCharacterMovement();

	HealthComponent->OnHealthChangedDelegate.AddDynamic(this, &AUB_Character::OnHealthChanged);
	HealthComponent->OnDeadDelegate.AddDynamic(this, &AUB_Character::OnDead);

	GetWorldTimerManager().SetTimer(TimerHandle_Interaction, this, &AUB_Character::TickInteraction, TickInteractionFrequency, true);

	//Always at the end
	VerifyData();
}

void AUB_Character::InitializeReferences()
{
	GameModeReference = Cast<AUB_GameMode>(GetWorld()->GetAuthGameMode());
	GameInstanceReference = Cast<UUB_GameInstance>(GetWorld()->GetGameInstance());
}

void AUB_Character::SetupCharacterMovement()
{
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	MaxCrouchSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;
	//StandingCapsuleHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	ECurrentMovementState = EMovementState::Standing;

	bIsPressingSprint = false;
	bToggleSprintState = false;
	bIsPressingCrouchOrSlide = false;
	ResolveMaxMovementSpeed(true);
	ResolveMaxCrouchMovementSpeed();

	SetSpeedModifierToDefault();
}

// Called every frame
void AUB_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	PlayerInputComponent->BindAction("AdditionalWeaponAction", IE_Pressed, this, &AUB_Character::StartAdditionalWeaponAction);
	PlayerInputComponent->BindAction("ChangeWeaponMode", IE_Pressed, this, &AUB_Character::ChangeWeaponMode);

	PlayerInputComponent->BindAction("WeaponPunchAction", IE_Pressed, this, &AUB_Character::StartWeaponPunchAction);
	//PlayerInputComponent->BindAction("WeaponPunchAction", IE_Released, this, &AUB_Character::StopWeaponPunchAction);

	PlayerInputComponent->BindAction("ThrowAction", IE_Pressed, this, &AUB_Character::StartThrowAction);
	PlayerInputComponent->BindAction("ThrowAction", IE_Released, this, &AUB_Character::StopThrowAction);

	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &AUB_Character::StartUltimate);
	//PlayerInputComponent->BindAction("Ultimate", IE_Released, this, &AUB_Character::StopUltimate);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUB_Character::StartInteraction);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AUB_Character::StopInteraction);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AUB_Character::PauseGame).bExecuteWhenPaused = true;
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

	//then slide
	float currentSpeed = GetVelocity().Size(); //magnitude
	CurrentSlideDuration = (currentSpeed*MinSlideDuration)/MinSpeedToSlide;
	GetWorldTimerManager().SetTimer(TimerHandle_Slide, this, &AUB_Character::StopSliding, CurrentSlideDuration, false);

	ECurrentMovementState = EMovementState::Sliding;
	//as this crouch is controlled by character movement change max crouch speed
	ResolveMaxCrouchMovementSpeed();
}

void AUB_Character::StopSliding()
{
	bIsPressingCrouchOrSlide = false; //Not allow player to continue sliding, if the time was over
	StopCrouching(); //Slide is actually collide as crouching then stop crouching

	ResolveMaxCrouchMovementSpeed(); //return crouch speed to its default value
	GetWorldTimerManager().ClearTimer(TimerHandle_Slide);
}

//Movement
void AUB_Character::ResolveMovement() //Reset, if user pressed keys or was pressing
{
	if (ECurrentMovementState == EMovementState::Standing) {
		if (bIsPressingCrouchOrSlide) {
			float CurrentSpeed = GetVelocity().Size(); //magnitude

			if (CurrentSpeed >= MinSpeedToSlide) {
				Slide();
			}
			else {
				Crouch();
			}
		}
	}

	ResolveMaxMovementSpeed();
}
void AUB_Character::ResolveMaxMovementSpeed(bool forceUpdate)
{
	bool updateWalkSpeed = false; //do not update always just when necessary

	float currentMaxSpeed;
	if (ECurrentMovementState == EMovementState::Standing) {
		if (bUseHoldToSprint) {
			currentMaxSpeed = bIsPressingSprint ? MaxRunSpeed : MaxWalkSpeed;
		}
		else {
			currentMaxSpeed = bToggleSprintState ? MaxRunSpeed : MaxWalkSpeed;
		}

		updateWalkSpeed = true;
	}

	if (updateWalkSpeed || forceUpdate) {
		GetCharacterMovement()->MaxWalkSpeed = currentMaxSpeed * SpeedModifier;
	}
}
void AUB_Character::ResolveMaxCrouchMovementSpeed()
{
	float currentMaxCrouchSpeed;
	if (ECurrentMovementState == EMovementState::Sliding) {
		currentMaxCrouchSpeed = MaxSlideSpeed;
	}
	else {
		currentMaxCrouchSpeed = MaxCrouchSpeed; //return crouch speed to its default value
	}

	GetCharacterMovement()->MaxWalkSpeedCrouched = currentMaxCrouchSpeed * SpeedModifier;
}

//Interaction
void AUB_Character::TickInteraction()
{
	//TODO: Maybe show a message like 'press E'' when pointing to an interactiveObject...
	IUB_InteractiveItemInterface* NewAimingAt;
	if (IsAimingAtAnyInteractiveItem(NewAimingAt)) {
		if (NewAimingAt != AimingAtItem) 
		{
			StopInteraction();
			if (AimingAtItem) {
				IUB_InteractiveItemInterface::Execute_StopHover(Cast<UObject>(AimingAtItem));
			}
			if (NewAimingAt) {
				IUB_InteractiveItemInterface::Execute_StartHover(Cast<UObject>(NewAimingAt));
			}
			AimingAtItem = NewAimingAt;
		}
	}
	else {
		if (AimingAtItem) {
			IUB_InteractiveItemInterface::Execute_StopHover(Cast<UObject>(AimingAtItem));
		}
		StopInteraction();
		AimingAtItem = nullptr;
	}

	//Validate if it's still focusing the object, if not cancel interaction
	/*if (bIsInteracting && AimingAtItem) {
		IUB_InteractiveItemInterface* NewAimingAt;
		if (IsAimingAtAnyInteractiveItem(NewAimingAt)) {
			if (NewAimingAt != AimingAtItem) {
				StopInteraction();
			}
		}
		else {
			StopInteraction();
		}
	}*/
}

void AUB_Character::StartInteraction()
{
	bIsInteracting = IsAimingAtAnyInteractiveItem(AimingAtItem);
	if (bIsInteracting && AimingAtItem) {
		IUB_InteractiveItemInterface::Execute_StartInteraction(Cast<UObject>(AimingAtItem), this);
	}
}
void AUB_Character::StopInteraction()
{
	if (bIsInteracting && AimingAtItem) {
		//InteractingWithItem->Execute_StopInteraction(this);
		IUB_InteractiveItemInterface::Execute_StopInteraction(Cast<UObject>(AimingAtItem), this);
		bIsInteracting = false;
	}
}

bool AUB_Character::IsAimingAtAnyInteractiveItem(IUB_InteractiveItemInterface*& InteractiveItem)
{
	FHitResult HitResult;

	FVector EyeLocation;
	FRotator EyeRotation;
	GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector AimDirection = EyeRotation.Vector();
	FVector TraceEnd = EyeLocation + (AimDirection * MaxDistanceToInteract);

	FCollisionQueryParams QueryParams;
	if(IsValid(CurrentWeapon)) QueryParams.AddIgnoredActor(CurrentWeapon); //ignore my weapon
	QueryParams.AddIgnoredActor(this); //ignore myself
	QueryParams.bTraceComplex = true; //to be able to get more data, and more accurate collision

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, TCC_INTERACTION, QueryParams);

	if (bHit) {
		AActor* HitActor = HitResult.GetActor();
		if (IsValid(HitActor)) {
			InteractiveItem = Cast<IUB_InteractiveItemInterface>(HitActor);
			if (InteractiveItem) {
				return true;
			}
			else { //if it comes from BP
				if (HitActor->GetClass()->ImplementsInterface(UUB_InteractiveItemInterface::StaticClass())) {
					return true;
				}
			}
		}
	}

	if (bDebugInteractionLineTrace) {
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Magenta, false, 1.0f, 0.0f, 1.0f);
	}

	return false;
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
	//Inventory = NewObject<UUB_CharacterInventory>(); //not now, using TSubclassOf
	if (IsValid(InventoryClass)) {
		Inventory = InventoryClass->GetDefaultObject<UUB_CharacterInventory>();
	}
}

//Weapon
void AUB_Character::CreateInitialWeapon()
{
	if (IsValid(InitialWeaponClass)) {
		//CurrentWeapon = GetWorld()->SpawnActor<AUB_Weapon>(InitialWeaponClass, GetActorLocation(), GetActorRotation());
		AUB_Weapon* NewWeapon  = GetWorld()->SpawnActor<AUB_Weapon>(InitialWeaponClass);
		EquipWeapon(NewWeapon);
	}
}
void AUB_Character::EquipWeapon(AUB_Weapon* Weapon) //TODO: Use later with 1 2 3 keys
{
	CurrentWeapon = Weapon;

	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->SetCharacterOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		CurrentWeapon->EquipWeapon();
	}
}

//Weapon Action
void AUB_Character::StartWeaponAction()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StartAction(); //this method could change the action state
	}
}
void AUB_Character::StopWeaponAction()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StopAction();
	}
}

//Additional Weapon Action
void AUB_Character::StartAdditionalWeaponAction()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StartAdditionalAction();
	}
}

//Punch Action
void AUB_Character::StartWeaponPunchAction()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StartPunchAction();
	}
}

//Change Weapon Mode
void AUB_Character::ChangeWeaponMode()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->ChangeWeaponMode();
	}
}

//Throw Action
void AUB_Character::StartThrowAction()
{
	if (!IsValid(InitialThrowableItemClass)) return;

	float ProjectileRadius = 0.0f;
	AUB_Projectile* ThrowableItemBase = InitialThrowableItemClass->GetDefaultObject<AUB_Projectile>();
	if (IsValid(ThrowableItemBase)) {
		ProjectileRadius = ThrowableItemBase->GetRadius();
	}

	const TArray<AActor*> IgnoreActors = { this, CurrentWeapon };

	DrawProjectiletTrajectoryComponent->ShowTrajectoryPrediction(ThrowSpeed, ProjectileRadius, IgnoreActors);

	/*FHitResult HitResult;
	FOccluderVertexArray PathPoints;
	FVector LastTraceDestination;

	FVector StartPosition = GetActorLocation();

	FVector EyeLocation;
	FRotator EyeRotation;
	GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector ThrowVelocity = EyeRotation.Vector() * ThrowSpeed;

	AUB_ThrowableItem* ThrowableItemBase = InitialThrowableItemClass->GetDefaultObject<AUB_ThrowableItem>();
	float ProjectileRadius = ThrowableItemBase->GetRadius();

	const TArray<AActor*> IgnoreActors = { this, CurrentWeapon }; //grenade

	bool bHit = UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), HitResult, PathPoints, LastTraceDestination, StartPosition, ThrowVelocity,
		true, ProjectileRadius, ECollisionChannel::ECC_Visibility, false, IgnoreActors, EDrawDebugTrace::ForDuration, 3.0f);*/
}
void AUB_Character::StopThrowAction()
{
	if (!IsValid(InitialThrowableItemClass)) return;

	DrawProjectiletTrajectoryComponent->HideTrajectoryPrediction();
	ThrowItem();
}
void AUB_Character::ThrowItem()
{
	if (!IsValid(InitialThrowableItemClass)) return;

	//at this moment we just instantiate an actor with the InitialThrowableItemClass, maybe in the future it could be the one equipped in the player
	AUB_Projectile* ThrowItem = GetWorld()->SpawnActor<AUB_Projectile>(InitialThrowableItemClass, GetStartLocationThrow(), FRotator::ZeroRotator);
	if (IsValid(ThrowItem)) {
		ThrowItem->SetCharacterOwner(this);
		ThrowItem->SetProjectileVelocity(GetThrowVelocity());
		ThrowItem->Throw();
	}
	
}
FVector AUB_Character::GetStartLocationThrow() const {
	//return ReferenceStartLocationThrow->GetComponentLocation();
	//TODO use a socket or something
	return GetActorLocation() + FVector(-100.0f, 100.0f, 100.0f);
}
FVector AUB_Character::GetThrowVelocity() const
{
	FVector EyeLocation;
	FRotator EyeRotation;
	GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector ThrowDirection = EyeRotation.Vector();

	const FVector ThrowVelocity = ThrowDirection * ThrowSpeed;
	return ThrowVelocity;
}

//Anim notifiers
void AUB_Character::OnFinishedWeaponAction()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->OnFinishedAction();
	}
}
void AUB_Character::OnFinishedAdditionalWeaponAction()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->OnFinishedAdditionalAction();
	}
}
void AUB_Character::OnFinishedWeaponPunchAction()
{
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->OnFinishedPunchAction();
	}
}

//MeleeCombo anim notifiers
void AUB_Character::ANEnableMeleeCombo()
{
	if (IsValid(CurrentWeapon)) {
		AUB_MeleeWeapon* MeleeWeapon = Cast<AUB_MeleeWeapon>(CurrentWeapon);
		if (IsValid(MeleeWeapon)) {
			MeleeWeapon->SetMeleeComboEnabled(true);
		}
	}
}
void AUB_Character::ANResetMeleeCombo()
{
	if (IsValid(CurrentWeapon)) {
		AUB_MeleeWeapon* MeleeWeapon = Cast<AUB_MeleeWeapon>(CurrentWeapon);
		if (IsValid(MeleeWeapon)) {
			MeleeWeapon->ResetMeleeCombo();
		}
	}
}

//Ultimate
void AUB_Character::InitializeUltimate()
{
	if (IsValid(UltimateClass)) {
		CurrentUltimate = GetWorld()->SpawnActor<AUB_CharacterUltimate>(UltimateClass);
		if (IsValid(CurrentUltimate)) {
			CurrentUltimate->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			CurrentUltimate->SetCharacterOwner(this);
		}
	}
	ResetUltimateXP();
}
void AUB_Character::EarnUltimateXP(float XP)
{
	if (bCanUseUltimate || bIsUsingUltimate) return;

	CurrentUltimateXP = FMath::Clamp(CurrentUltimateXP + XP, 0.0f, MaxUltimateXP);
	if (CurrentUltimateXP >= MaxUltimateXP) {
		bCanUseUltimate = true;
	}

	BP_EarnUltimateXP(XP);
}
void AUB_Character::StartUltimate()
{
	if (bCanUseUltimate && !bIsUsingUltimate) {
		bCanUseUltimate = false;
		bIsUsingUltimate = true;

		if (IsValid(CurrentUltimate)) {
			CurrentUltimate->StartUltimate();
		}
	}
}
void AUB_Character::OnFinishedUltimate()
{
	bIsUsingUltimate = false;
	ResetUltimateXP();
}
void AUB_Character::ResetUltimateXP() {
	CurrentUltimateXP = 0.0f;
}

//Speed modifier
void AUB_Character::SetSpeedModifier(float SpeedMod) {
	SpeedModifier = SpeedMod;

	//Update movement speeds
	ResolveMaxMovementSpeed(true);
	ResolveMaxCrouchMovementSpeed();
}
void AUB_Character::SetSpeedModifierToDefault() {
	SetSpeedModifier(1.0f);
}

//Animation
float AUB_Character::PlayAnimMontage(UAnimMontage* Montage, bool bIsFullBody, float InPlayRate, FName StartSectionName)
{
	bIsFullBodyAnimation = bIsFullBody;
	const float AnimDuration = Super::PlayAnimMontage(Montage, InPlayRate, StartSectionName);

	if (bIsFullBodyAnimation) {
		//Wait until animation ends and reset fullBodyAnimation flag
		GetWorldTimerManager().SetTimer(TimerHandle_FullBodyAnimation, this, &AUB_Character::ResetFullBodyAnimationFlag, AnimDuration, false);
	}

	return AnimDuration;
}
void AUB_Character::ResetFullBodyAnimationFlag()
{
	bIsFullBodyAnimation = false;
}
void AUB_Character::PlaySectionAnimMontage(FName Section, const UAnimMontage* Montage)
{
	bIsFullBodyAnimation = false; //at this moment anim section is always not fullbody
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) && IsValid(Montage)) {
		AnimInstance->Montage_JumpToSection(Section, Montage);
	}
}

//Health
void AUB_Character::OnHealthChanged(UUB_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Value, const UDamageType* DamageType, AController* InstigatedBy, AActor* ActorCauser)
{
	//....
}
bool AUB_Character::TryGiveHealth(float ExtraHealth, AController* InstigatedBy, AActor* ActorCauser) {
	return HealthComponent->TryGiveHealth(ExtraHealth, InstigatedBy, ActorCauser);
}
void AUB_Character::OnDead(AActor* ActorCauser)
{
	if (GetCharacterType() == EUB_CharacterType::CharacterType_Player) {
		if (IsValid(GameModeReference)) {
			GameModeReference->GameOver(this);
		}
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
}
bool AUB_Character::IsDead() const {
	return HealthComponent->IsDead();
}
bool AUB_Character::HasFullHealth() const {
	return HealthComponent->HasFullHealth();
}

//General
void AUB_Character::PauseGame()
{
	if (IsValid(GameModeReference)) {
		GameModeReference->TogglePauseGame();
	}
}

//Data
void AUB_Character::VerifyData()
{
	if (!IsValid(CurrentWeapon)) UE_LOG(LogTemp, Warning, TEXT("Current weapon was not defined"));
	if (!IsValid(Inventory)) UE_LOG(LogTemp, Error, TEXT("Character inventory was not created correctly"));
	if (!IsValid(CurrentUltimate)) UE_LOG(LogTemp, Error, TEXT("Character ultimate was not created correctly"));

	if (MaxWalkSpeed <= 0) UE_LOG(LogTemp, Error, TEXT("MaxWalkSpeed setted in CharacterMovementComponent is zero or less than zero"));
	if (MaxCrouchSpeed <= 0) UE_LOG(LogTemp, Error, TEXT("MaxCrouchSpeed setted in CharacterMovementComponent is zero or less than zero"));
}
