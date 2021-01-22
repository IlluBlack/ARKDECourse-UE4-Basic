// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_CollectableItem.h"
#include "UB_Character.h"
#include "Components/SphereComponent.h"

// Sets default values
AUB_CollectableItem::AUB_CollectableItem()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MainColliderComponent"));
	MainColliderComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Physic collision with world
	MainColliderComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	//Enable collisions
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//Set as RootComponent, this way we can use NotifyActorBeginOverlap()
	RootComponent = MainColliderComponent; 

	XPValue = 20.0f;
	bShouldDestroyWhenPickedUp = true;
}

// Called when the game starts or when spawned
void AUB_CollectableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUB_CollectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUB_CollectableItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsValid(OtherActor)) {
		AUB_Character* OverlappedCharacter = Cast<AUB_Character>(OtherActor);

		if (IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType() == EUB_CharacterType::CharacterType_Player) 
		{ //if it's actually the player
			Pickup(OverlappedCharacter);
		}
	}
}

void AUB_CollectableItem::Pickup(AUB_Character* PickupCharacter)
{
	PickupCharacter->EarnUltimateXP(XPValue); //Collectables give XP
	BP_Pickup(PickupCharacter);

	if(bShouldDestroyWhenPickedUp)
		Destroy();
}
