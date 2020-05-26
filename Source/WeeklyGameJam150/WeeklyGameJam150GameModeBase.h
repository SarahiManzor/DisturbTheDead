// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WeeklyGameJam150GameModeBase.generated.h"

class AMainCharacter;
class ADoor;

USTRUCT()
struct FLevelInstance
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int TreasureToWin;

	UPROPERTY(VisibleAnywhere)
	ADoor* DoorToNextLevel;
};

USTRUCT()
struct FInstruction
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FString Instruction;

	UPROPERTY(VisibleAnywhere)
	bool bCanSkip;

	UPROPERTY(VisibleAnywhere)
	bool bDontAppend;
};

/**
 * 
 */
UCLASS()
class WEEKLYGAMEJAM150_API AWeeklyGameJam150GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AWeeklyGameJam150GameModeBase();

		// ----------Base Functions----------
public:
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ----------Variables----------
public:
	UPROPERTY(VisibleAnywhere)
	TMap<int, FLevelInstance> Levels;

	FInstruction CurrentInstruction;
protected:
private:
	AMainCharacter* Player;

	int CurrentLevel = 0;

	TArray<FInstruction> Instructions;
	int CurrentInstructionIndex = -1;

	int ResetIndex = 0;

	// ----------Custom Functions----------
public:
	FORCEINLINE void NextLevel() { CurrentLevel ++; }
	void NextInstruction(bool Forced = false);
	void SkipNextInstruction();
	void SetResetIndex();
	FORCEINLINE int32 GetLevelIndex() { return CurrentLevel; }
	void FailLevel();
protected:
private:
	void TrySetPlayer();

	void GetInstructions();
};
