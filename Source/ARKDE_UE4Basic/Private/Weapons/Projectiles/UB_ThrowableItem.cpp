// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_ThrowableItem.h"
#include "UB_Character.h"

#include "Engine/World.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Hearing.h"


AUB_ThrowableItem::AUB_ThrowableItem()
{

}

void AUB_ThrowableItem::BeginPlay()
{
	Super::BeginPlay();
	bNotifiedNoise = false;
}

void AUB_ThrowableItem::NotifyAINoiseEvent()
{
	//PLAY SOUND
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, CharacterOwner);
	bNotifiedNoise = true;

	//Destroy
	SetLifeSpan(2.0f);
}

void AUB_ThrowableItem::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bNotifiedNoise) return;

	if (Other != CharacterOwner) {
		NotifyAINoiseEvent();
		UE_LOG(LogTemp, Log, TEXT("Notified AI Noise Event"));
	}
}
