// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "WeeklyGameJam150GameModeBase.h"
#include "Engine/World.h"
#include "MainCharacter.h"
#include "Door.h"

AWeeklyGameJam150GameModeBase::AWeeklyGameJam150GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeeklyGameJam150GameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void AWeeklyGameJam150GameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//UE_LOG(LogTemp, Warning, TEXT("TickTick"));

	if (!Player)
	{
		TrySetPlayer();
	}
	else
	{
		if (Levels.Num() > CurrentLevel && Player->GetTotalCollected() >= Levels[CurrentLevel].TreasureToWin)
		{
			// Todo: Proceed to next level / Open door or something / Reset Ghosts
			Levels[CurrentLevel].DoorToNextLevel->SetDoorOpen(true);
		}
	}
}

void AWeeklyGameJam150GameModeBase::TrySetPlayer()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APawn* Pawn = PC->GetPawn();
		if (Pawn)
		{
			AMainCharacter* MainPlayer = Cast<AMainCharacter>(Pawn);
			if (MainPlayer)
			{
				Player = MainPlayer;
			}
		}
	}
}

void AWeeklyGameJam150GameModeBase::FailLevel()
{
	if (Levels.Num() > CurrentLevel)
	{
		// Todo: Proceed to next level / Open door or something / Reset Ghosts
		Levels[CurrentLevel].DoorToNextLevel->SetDoorOpen(false);
	}
}