// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UB_HealthComponent.generated.h"

//Delegate signature
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UUB_HealthComponent*, HealthComponent, AActor*, AffectedActor, float, Value, const UDamageType*, DamageType, AController*, InstigatedBy, AActor*, ActorCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARKDE_UE4BASIC_API UUB_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health Component")
	float Health; //CurrentHealth
	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
	AActor* Owner;

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
	bool bIsDead;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebug;

public:
	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChangedDelegate;

public:	
	// Sets default values for this component's properties
	UUB_HealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ResetHealth();
	UFUNCTION()
	void NotifyOnTakeDamage(AActor* AffectedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const { return Health; };
	UFUNCTION(BlueprintCallable)
	float HasFullHealth() const { return Health >= MaxHealth; };

	UFUNCTION(BlueprintCallable)
	void GiveHealth(float ExtraHealth, AController* InstigatedBy, AActor* ActorCauser);

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return bIsDead; };
};
