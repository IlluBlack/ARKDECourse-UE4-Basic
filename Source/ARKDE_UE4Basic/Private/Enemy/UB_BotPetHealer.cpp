// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_BotPetHealer.h"
#include "UB_Character.h"
#include "UB_BotPetControlBase.h"
#include "Components/UB_HealthComponent.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "DrawDebugHelpers.h"

AUB_BotPetHealer::AUB_BotPetHealer()
{
	HealthRegenerationAreaComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SelfDestructionDetectorComponent"));
	HealthRegenerationAreaComponent->SetupAttachment(MainColliderComponent);
	HealthRegenerationAreaComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	HealingParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HealingParticlesComponent"));
	HealingParticlesComponent->SetupAttachment(MainColliderComponent);
	HealingParticlesComponent->SetAutoActivate(false);
	HealingParticlesComponent->bAbsoluteRotation = true; //do not rotate this particles with bot pet

	HealthRegenerationFrequency = 0.3f;
	HealthRegenerationRate = 5.0f;

	MaxDistanceOutOfControlBase = 45.0f;

	InternalRadiusParticlesParameterName = "InternalRadius";
	InternalRadiusParticlesBaseUnit = 0.75f;
	ExternalRadiusParticlesParameterName = "ExternalRadius";
	ExternalRadiusParticlesBaseUnit = 0.6f;
}

void AUB_BotPetHealer::BeginPlay()
{
	Super::BeginPlay();

	HealthRegenerationAreaComponent->OnComponentBeginOverlap.AddDynamic(this, &AUB_BotPetHealer::OnOtherActorEnterRegenerationArea);
	HealthRegenerationAreaComponent->OnComponentEndOverlap.AddDynamic(this, &AUB_BotPetHealer::OnOtherActorExitRegenerationArea);

	const float RadiusInternal = MainColliderComponent->GetScaledSphereRadius() * InternalRadiusParticlesBaseUnit;
	HealingParticlesComponent->SetVectorParameter(InternalRadiusParticlesParameterName, FVector(RadiusInternal, RadiusInternal, RadiusInternal));
	
	const float RadiusExternal = HealthRegenerationAreaComponent->GetScaledSphereRadius() * ExternalRadiusParticlesBaseUnit;
	HealingParticlesComponent->SetVectorParameter(ExternalRadiusParticlesParameterName, FVector(RadiusExternal, RadiusExternal, RadiusExternal));

	HealingParticlesComponent->DeactivateSystem();

	bIsRegeneratingHealth = false;

	/*if (IsValid(ControlBase)) {
		ControlBase->SuscribeBotPet(this);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("UBotPetHealer has no control base assigned, it will heal but not navigate"));
	}*/
}

void AUB_BotPetHealer::Tick(float DeltaTime)
{
	if (CharactersInRegenerationArea.Num() > 0) {
		return; //do not move, and stay with characters around, just leave when noone around
	}

	if (IsValid(CurrentTargetCharacter)) {
		if (CurrentTargetCharacter->IsDead() || CurrentTargetCharacter->GetHealthComponent()->HasFullHealth()) {
			CurrentTargetCharacter = nullptr;
		}
		//Verify if it's sill in the control base area
		if (IsValid(CurrentTargetCharacter) && IsValid(ControlBase)) {
			float DistanceFromControlBase = (GetActorLocation() - ControlBase->GetActorLocation()).Size();
			float DistanceOutOfControlBase = DistanceFromControlBase - ControlBase->GetControlRadius();
			if (DistanceOutOfControlBase > 0.0f && DistanceOutOfControlBase > MaxDistanceOutOfControlBase) {
				//is out of controlBase and should return to base, ignore this target
				CurrentTargetCharacter = nullptr;
			}
		}
	}

	if (!IsValid(CurrentTargetCharacter)) {
		CurrentTargetCharacter = GetNextCharacterNeedingHealth();
	}

	NextPathPoint = GetNextPathPoint(); //update always

	const float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
	if (DistanceToTarget > MinDistanceToTarget) { 
		MoveToNextPathPoint();
	}

	if (bDrawNextPathPoint) {
		DrawDebugSphere(GetWorld(), NextPathPoint, 30.0f, 15.0f, FColor::Purple, false, 0.0f, 0, 1.0f);
	}
}

void AUB_BotPetHealer::OnOtherActorEnterRegenerationArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor)) {
		AUB_Character* OtherCharacter = Cast<AUB_Character>(OtherActor);
		if (IsValid(OtherCharacter)) {
			if (!OtherCharacter->GetHealthComponent()->HasFullHealth()) {
				CharactersInRegenerationArea.Add(OtherCharacter);
				VerifyHealthRegenerationTimer();
			}
		}
	}
}

void AUB_BotPetHealer::OnOtherActorExitRegenerationArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor)) {
		AUB_Character* OtherCharacter = Cast<AUB_Character>(OtherActor);
		if (IsValid(OtherCharacter)) {
			CharactersInRegenerationArea.Remove(OtherCharacter); //remove if exists
			VerifyHealthRegenerationTimer();
		}
	}
}

void AUB_BotPetHealer::VerifyHealthRegenerationTimer()
{
	if (CharactersInRegenerationArea.Num() > 0 && !bIsRegeneratingHealth) {
		GetWorldTimerManager().SetTimer(TimerHandle_HealthRegeneration, this, &AUB_BotPetHealer::TickHealthRegeneration, HealthRegenerationFrequency, true);
		bIsRegeneratingHealth = true;

		//Activate FX
		HealingParticlesComponent->ActivateSystem();
	}
	else if (CharactersInRegenerationArea.Num() <= 0 && bIsRegeneratingHealth) {
		GetWorldTimerManager().ClearTimer(TimerHandle_HealthRegeneration);
		bIsRegeneratingHealth = false;

		//Deactivate Fx
		HealingParticlesComponent->DeactivateSystem();
	}
}

void AUB_BotPetHealer::TickHealthRegeneration()
{
	bIsRegeneratingHealth = true;

	//In reverse because it could remove an item while iterating
	for (int idx = CharactersInRegenerationArea.Num() - 1; idx >= 0; idx--) {
		AUB_Character* Character = CharactersInRegenerationArea[idx];

		bool bShouldRemoveCharacter = false;
		if (IsValid(Character)) {
			if (Character->IsDead() || Character->GetHealthComponent()->HasFullHealth()) {
				bShouldRemoveCharacter = true;
			}
			else {
				Character->TryGiveHealth(HealthRegenerationRate, GetInstigatorController(), this);
			}
		}
		else {
			bShouldRemoveCharacter = true;
		}

		if (bShouldRemoveCharacter) {
			CharactersInRegenerationArea.Remove(Character);
			VerifyHealthRegenerationTimer();
		}
	}
}

//Navigation
FVector AUB_BotPetHealer::GetNextPathPoint()
{
	if (IsValid(CurrentTargetCharacter)) {
		return Super::GetNextPathPoint(); //go to this target
	}

	//if not, then go to the base
	if (IsValid(ControlBase)) {
		const FVector RestSeat = ControlBase->GetLocationRestSeat(this);
		UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), RestSeat);
		if (NavigationPath->PathPoints.Num() > 1) { //0 is always the StartPosition
			return NavigationPath->PathPoints[1]; //nextPoint
		}
	}

	return GetActorLocation();
}

AUB_Character* AUB_BotPetHealer::GetNextCharacterNeedingHealth()
{
	if (IsValid(ControlBase)) {
		const TArray<AUB_Character*> AvailableCharacters = ControlBase->GetCharactersInControlRadius();
		if (AvailableCharacters.Num() <= 0) return nullptr;

		AUB_Character* ClosestCharacter = nullptr;
		float ShortestPathLength = -1.0f;
		for (AUB_Character* AvailableCharacter : AvailableCharacters)
		{
			if (!IsValid(AvailableCharacter)) continue;
			if (AvailableCharacter->GetHealthComponent()->HasFullHealth()) continue;

			float PathLength;
			UNavigationSystemV1::GetPathLength(GetWorld(), GetActorLocation(), AvailableCharacter->GetActorLocation(), PathLength);

			if (ShortestPathLength <= 0.0f) { //first time in the for
				ShortestPathLength = PathLength;
			}

			if (PathLength <= ShortestPathLength) {
				ShortestPathLength = PathLength;
				ClosestCharacter = AvailableCharacter;
			}
		}

		return ClosestCharacter;
	}

	return nullptr;
}
