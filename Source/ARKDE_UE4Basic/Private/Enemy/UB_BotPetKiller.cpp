// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_BotPetKiller.h"
#include "UB_Character.h"

#include "Components/SphereComponent.h"
#include "Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"

AUB_BotPetKiller::AUB_BotPetKiller()
{
	SelfDestructionDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SelfDestructionDetectorComponent"));
	SelfDestructionDetectorComponent->SetupAttachment(MainColliderComponent);
	SelfDestructionDetectorComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SelfDestructionDetectorComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SelfDestructionDetectorComponent->SetSphereRadius(150.0f);

	SelfDestructionCountdownFrequency = 0.5f;
	SelfDestructionCountdownDamage = 20.0f;
}

void AUB_BotPetKiller::BeginPlay()
{
	Super::BeginPlay();

	SelfDestructionDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AUB_BotPetKiller::OnOtherActorsDetection);

	//Target is going to be the player
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn)) {
		CurrentTargetCharacter = Cast<AUB_Character>(PlayerPawn);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn not found"));
	}

	NextPathPoint = GetNextPathPoint();
}

void AUB_BotPetKiller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//...
}

//SelfDestructionDetection
void AUB_BotPetKiller::OnOtherActorsDetection(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == CurrentTargetCharacter) {
		if (bStartedSelfDestructionCountdown) return;
		bStartedSelfDestructionCountdown = true;

		GetWorldTimerManager().SetTimer(TimerHandle_SelfDestructionCountdown, this, &AUB_BotPetKiller::SelfDamage, SelfDestructionCountdownFrequency, true);
	}
}
void AUB_BotPetKiller::SelfDamage()
{
	UGameplayStatics::ApplyDamage(this, SelfDestructionCountdownDamage, GetInstigatorController(), nullptr, nullptr);
}
