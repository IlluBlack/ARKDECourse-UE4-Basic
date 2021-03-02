// Fill out your copyright notice in the Description page of Project Settings.


#include "UB_CharacterInventory.h"

UUB_CharacterInventory::UUB_CharacterInventory()
{

}

void UUB_CharacterInventory::AddKey(FName NewKey) {
	Keys.Add(NewKey);
}
bool UUB_CharacterInventory::HasKey(FName KeyTag) {
	return Keys.Contains(KeyTag);
}
bool UUB_CharacterInventory::TryUseKey(FName KeyTag) {
	if (HasKey(KeyTag)) {
		Keys.Remove(KeyTag);
		return true;
	}
	return false;
}
