// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_TriggerItem.h"
#include "UB_Character.h"
#include "Components/BoxComponent.h"

// Sets default values
AUB_TriggerItem::AUB_TriggerItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MainColliderComponent"));
	MainColliderComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Overlap Pawns
	MainColliderComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	//Enable collisions
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//Set as RootComponent, this way we can use NotifyActorBeginOverlap()
	RootComponent = MainColliderComponent;

}

// Called when the game starts or when spawned
void AUB_TriggerItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUB_TriggerItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUB_TriggerItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsValid(OtherActor)) {
		AUB_Character* OverlappedCharacter = Cast<AUB_Character>(OtherActor);

		if (IsValid(OverlappedCharacter)) { //if it's actually the player
			OnCharacterOverlap(OverlappedCharacter);
		}
	}
}

void AUB_TriggerItem::OnCharacterOverlap(AUB_Character*Character)
{
	BP_OnCharacterOverlap(Character);
}
