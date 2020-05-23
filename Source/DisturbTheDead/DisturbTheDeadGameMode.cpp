// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DisturbTheDeadGameMode.h"
#include "DisturbTheDeadPlayerController.h"
#include "DisturbTheDeadCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADisturbTheDeadGameMode::ADisturbTheDeadGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADisturbTheDeadPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}