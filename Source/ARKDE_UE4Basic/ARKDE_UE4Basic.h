// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Object Collision Channels: OCC
#define OCC_ENEMY ECC_GameTraceChannel1

//Trace Collision Channels: TCC
#define TCC_WEAPON ECC_GameTraceChannel2
#define TCC_INTERACTION ECC_GameTraceChannel3

//Actor Tags
const FName TAG_ENEMY = "Enemy";