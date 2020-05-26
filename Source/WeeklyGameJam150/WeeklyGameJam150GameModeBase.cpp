// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "WeeklyGameJam150GameModeBase.h"
#include "Engine/World.h"
#include "MainCharacter.h"
#include "Door.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

AWeeklyGameJam150GameModeBase::AWeeklyGameJam150GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeeklyGameJam150GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GetInstructions();
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

void AWeeklyGameJam150GameModeBase::SetResetIndex()
{
	ResetIndex = CurrentInstructionIndex;
}

void AWeeklyGameJam150GameModeBase::FailLevel()
{
	if (Levels.Num() > CurrentLevel)
	{
		// Todo: Proceed to next level / Open door or something / Reset Ghosts
		Levels[CurrentLevel].DoorToNextLevel->SetDoorOpen(false);
		CurrentInstructionIndex = ResetIndex - 1;
		NextInstruction(true);
	}
}

void AWeeklyGameJam150GameModeBase::GetInstructions()
{
	FString FileName = FPaths::ProjectContentDir() + "/Files/Instructions.txt";
	TArray<FString> FileResults;
	FFileHelper::LoadFileToStringArray(FileResults, *FileName);
	for (FString i : FileResults)
	{
		FInstruction Instruction;

		FString SkipText = "[CANSKIP]";
		FString AppendText = "[DONTAPPEND]";
		Instruction.bCanSkip = i.RemoveFromEnd(SkipText);
		Instruction.bDontAppend = i.RemoveFromEnd(AppendText);
		Instruction.Instruction = i;

		Instructions.Add(Instruction);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *i);
	}
}

void AWeeklyGameJam150GameModeBase::NextInstruction(bool Forced)
{
	if (CurrentInstruction.bDontAppend && !Forced) return;

	CurrentInstructionIndex++;

	if (CurrentInstructionIndex == 2)
	{
		Levels[CurrentLevel].DoorToNextLevel->SetDoorOpen(true);
	}

	if (CurrentInstructionIndex == 3)
	{
		Player->bCanDig = true;
	}


	if (CurrentInstructionIndex < Instructions.Num())
		CurrentInstruction = Instructions[CurrentInstructionIndex];
}

void AWeeklyGameJam150GameModeBase::SkipNextInstruction()
{
	NextInstruction();

	if (!CurrentInstruction.bDontAppend && CurrentInstructionIndex < Instructions.Num())
	{
		FInstruction NewInstruction;
		NewInstruction.bCanSkip = true;
		NewInstruction.bDontAppend = false;
		NewInstruction.Instruction = CurrentInstruction.Instruction + "";
		NewInstruction.Instruction += " You've already looted that one? Some luck you got there!";
		CurrentInstruction = NewInstruction;
	}
}