// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UB_CharacterInventory.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ARKDE_UE4BASIC_API UUB_CharacterInventory : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keys")
	TArray<FName> Keys;

public:
	UUB_CharacterInventory();

public: 
	void AddKey(FName NewKey);
	bool HasKey(FName KeyTag);
	bool TryUseKey(FName KeyTag);

};
