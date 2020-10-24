// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_GrenadeLauncher.h"
#include "UB_Projectile.h"
#include "Engine/World.h"

AUB_GrenadeLauncher::AUB_GrenadeLauncher()
{
}

void AUB_GrenadeLauncher::StartAction()
{
	Super::StartAction();

	AUB_Projectile* CurrentProjectile = GetWorld()->SpawnActor<AUB_Projectile>(ProjectileClass, GetMuzzleSocketLocation(), GetMuzzleSocketRotation());

}

void AUB_GrenadeLauncher::StopAction()
{
	Super::StopAction();
}
