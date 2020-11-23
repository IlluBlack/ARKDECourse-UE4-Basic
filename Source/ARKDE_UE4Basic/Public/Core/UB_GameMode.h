// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UB_GameMode.generated.h"

class AUB_Character;
class AUB_SpectatingCamera;

/**
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	AUB_SpectatingCamera* VictoryCamera;
	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	AUB_SpectatingCamera* GameOverCamera;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	float TimeToDissapearCharacter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spectating Camera")
	float SpectatingCameraBlendTime;

public:
	AUB_GameMode();

protected:
	virtual void BeginPlay() override;

	void SetupSpectatingCameras();
	void MoveCameraToSpectatingPoint(AUB_Character* Character, AUB_SpectatingCamera* SpectatingCamera);
	
public:
	void Victory(AUB_Character* Character);
	void GameOver(AUB_Character* Character);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BP_Victory(AUB_Character* Character);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BP_GameOver(AUB_Character* Character);
	
};
