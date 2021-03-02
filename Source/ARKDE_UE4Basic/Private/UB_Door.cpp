// Fill out your copyright notice in the Description page of Project Settings.
#include "UB_Door.h"
#include "UB_Character.h"
#include "UB_CharacterInventory.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AUB_Door::AUB_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot")); //returns pointer
	RootComponent = CustomRootComponent; //both are pointers, RootComponent default of unreal will be our customRootComponent

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrameComponent->SetupAttachment(CustomRootComponent);

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorComponent->SetupAttachment(CustomRootComponent);

	OpenDoorColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OpenDoorCollider"));
	OpenDoorColliderComponent->SetupAttachment(CustomRootComponent);
	OpenDoorColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OpenDoorColliderComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OpenDoorColliderComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	OpenWithKeyTag = "KeyA";
	OpenAngle = -90.0f;
}

// Called when the game starts or when spawned
void AUB_Door::BeginPlay()
{
	Super::BeginPlay();

	OpenDoorColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &AUB_Door::OnCollisionDetected);
}

// Called every frame
void AUB_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUB_Door::OnCollisionDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsOpen) return;

	if (IsValid(OtherActor)) {
		AUB_Character* OverlappedCharacter = Cast<AUB_Character>(OtherActor);

		if (IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType() == EUB_CharacterType::CharacterType_Player) 
		{
			UUB_CharacterInventory* PlayerInventory = OverlappedCharacter->GetInventory();
			if (IsValid(PlayerInventory)) {
				if (PlayerInventory->HasKey(OpenWithKeyTag)) {
					OpenDoor();
				}
			}
		}
	}
}

void AUB_Door::OpenDoor()
{
	/*FRotator rotation = FRotator(0.0f, OpenAngle, 0.0f);
	DoorComponent->SetRelativeRotation(rotation);*/

	bIsOpen = true;
	BP_OpenDoor();
}

