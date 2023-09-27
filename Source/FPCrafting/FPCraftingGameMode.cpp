// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPCraftingGameMode.h"
#include "FPCraftingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPCraftingGameMode::AFPCraftingGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
