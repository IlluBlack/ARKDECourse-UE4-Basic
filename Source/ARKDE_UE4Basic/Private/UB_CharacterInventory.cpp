// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_CharacterInventory.h"

UUB_CharacterInventory::UUB_CharacterInventory()
{

}

void UUB_CharacterInventory::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}
bool UUB_CharacterInventory::HasKey(FName KeyTag)
{
	return DoorKeys.Contains(KeyTag);
}
