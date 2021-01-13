// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UB_CharacterUltimate.h"
#include "UB_Ult_HighlightEnemies.generated.h"

/**
 * PostProcessing Highlight Objects: 
 - In Mesh/Rendering/Render CustomDepth = true
 - In Mesh/Rendering/CustomDepth Stencil Value:
	0 When you do not want to paint the object but keep it in front of the hightlight ones (the character and its weapon)
	1 Loot
	2 Enemies
	...
 * 
 */
UCLASS()
class ARKDE_UE4BASIC_API AUB_Ult_HighlightEnemies : public AUB_CharacterUltimate
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate")
	float DetectionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Debug")
	bool bDebugDetectionRadius;

public:
	AUB_Ult_HighlightEnemies();
	
protected:
	virtual void StartUltimateBehavior() override;

	void DetectEnemies();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_DisplayDetectedEnemies(int NumberOfEnemies);
};
