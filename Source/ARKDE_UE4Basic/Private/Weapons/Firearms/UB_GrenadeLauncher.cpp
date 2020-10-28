// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_GrenadeLauncher.h"
#include "UB_Projectile.h"
#include "Engine/World.h"

AUB_GrenadeLauncher::AUB_GrenadeLauncher()
{
}

void AUB_GrenadeLauncher::Fire()
{
	Super::Fire();

	AActor* Owner = GetOwner();
	if (IsValid(Owner)) {
		FVector EyeLocation;
		FRotator EyeRotation;
		Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		AUB_Projectile* CurrentProjectile = GetWorld()->SpawnActor<AUB_Projectile>(ProjectileClass, GetMuzzleSocketLocation(), EyeRotation);
		//AUB_Projectile* CurrentProjectile = GetWorld()->SpawnActor<AUB_Projectile>(ProjectileClass, GetMuzzleSocketLocation(), GetMuzzleSocketRotation());

		if (IsValid(CurrentProjectile)) {
			CurrentProjectile->SetOwner(Owner);
			CurrentProjectile->Throw(); //Notify projectile was thrown

			//TODO: override some caractheristics of this projectile?
		}

		PlayMuzzleEffect();
	}
}
