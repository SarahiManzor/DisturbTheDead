// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h" 
#include "WeeklyGameJam150GameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorFrameMesh->SetupAttachment(GetRootComponent());

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(DoorFrameMesh);

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Door %i"), DoorID);

	AWeeklyGameJam150GameModeBase* GameMode = Cast<AWeeklyGameJam150GameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		FLevelInstance LevelInstance = FLevelInstance();
		LevelInstance.DoorToNextLevel = this;
		LevelInstance.TreasureToWin = TreasureToOpen;

		GameMode->Levels.Add(DoorID, LevelInstance);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator TargetRotation = bIsOpen ? OpenRotation : CloseRotation;
	FRotator NewRotation = FMath::Lerp(DoorMesh->GetRelativeRotation(), TargetRotation, DeltaTime * DoorSpeed);
	DoorMesh->SetRelativeRotation(NewRotation);
}

void ADoor::OpenDoor()
{
	bIsOpen = true;
	UE_LOG(LogTemp, Warning, TEXT("Door %i opened!"), DoorID);
}

