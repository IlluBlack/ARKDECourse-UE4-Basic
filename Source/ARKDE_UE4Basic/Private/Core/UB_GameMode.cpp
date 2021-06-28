// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UB_GameMode.h"
#include "UB_Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UB_SpectatingCamera.h"
//#include "GameFramework/Controller.h"
//#include "GameFramework/PlayerController.h"

AUB_GameMode::AUB_GameMode()
{
	TimeToDissapearCharacter = 5.0f;
	SpectatingCameraBlendTime = 1.0f;
}

void AUB_GameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupSpectatingCameras();
}

void AUB_GameMode::SetupSpectatingCameras()
{
	TArray<AActor*> SpectatingCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUB_SpectatingCamera::StaticClass(), SpectatingCameraActors);

	if (SpectatingCameraActors.Num() > 0) {
		for (AActor* SpectatingCameraActor : SpectatingCameraActors) {
			AUB_SpectatingCamera* SpectatingCamera = Cast<AUB_SpectatingCamera>(SpectatingCameraActor);

			if (IsValid(SpectatingCamera)) {
				switch (SpectatingCamera->GetCameraType()) {
				case EUB_SpectatingCameraType::CameraType_Victory:
					VictoryCamera = SpectatingCamera;
					break;
				case EUB_SpectatingCameraType::CameraType_GameOver:
					GameOverCamera = SpectatingCamera;
					break;
				default:
					break;
				}
			}
		}
	}
}

void AUB_GameMode::Victory(AUB_Character* Character)
{
	Character->DisableInput(nullptr);
	MoveCameraToSpectatingPoint(Character, VictoryCamera);

	BP_Victory(Character);
}

void AUB_GameMode::GameOver(AUB_Character* Character)
{
	Character->GetMovementComponent()->StopMovementImmediately();
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (Character->ShouldDissapearWhenDead()) {
		Character->DetachFromControllerPendingDestroy();
		Character->SetLifeSpan(TimeToDissapearCharacter);
	}
	else {
		Character->DisableInput(nullptr);
		MoveCameraToSpectatingPoint(Character, GameOverCamera);
	}

	BP_GameOver(Character);
}

void AUB_GameMode::TogglePauseGame()
{
	APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);;
	if (IsValid(PlayerController)) {
		if (PlayerController->IsPaused()) {
			PauseGame(false);
		}
		else {
			PauseGame(true);
		}
	}
}

void AUB_GameMode::PauseGame(bool bNewState)
{
	APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);;
	if (IsValid(PlayerController)) {
		PlayerController->SetPause(bNewState);
		BP_PauseGame(bNewState);
	}
}

void AUB_GameMode::MoveCameraToSpectatingPoint(AUB_Character* Character, AUB_SpectatingCamera* SpectatingCamera)
{
	if (!IsValid(Character) || !IsValid(SpectatingCamera)) return;

	AController* CharacterController = Character->GetController();
	if (IsValid(CharacterController)) {
		APlayerController* PlayerController = Cast<APlayerController>(CharacterController); //to ensure it's not IA
		if (IsValid(PlayerController)) {
			PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingCameraBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
		}
	}
}
