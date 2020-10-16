// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_Rifle.h"

void AUB_Rifle::StartAction()
{
	Super::StartAction();

	UE_LOG(LogTemp, Log, TEXT("Start firing"));
}

void AUB_Rifle::StopAction()
{
	Super::StopAction();

	UE_LOG(LogTemp, Log, TEXT("Stop firing"));
}
