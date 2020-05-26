// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h" 
#include "WeeklyGameJam150GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "MainCharacter.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorFrameMesh->SetupAttachment(GetRootComponent());

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(DoorFrameMesh);

	CloseDoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CloseDoorTrigger"));
	CloseDoorTrigger->SetupAttachment(DoorFrameMesh);
	CloseDoorTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Door %i"), DoorId);

	AWeeklyGameJam150GameModeBase* GameMode = Cast<AWeeklyGameJam150GameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		FLevelInstance LevelInstance = FLevelInstance();
		LevelInstance.DoorToNextLevel = this;
		LevelInstance.TreasureToWin = TreasureToOpen;

		GameMode->Levels.Add(DoorId, LevelInstance);
	}

	CloseDoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::ComponentBeginOverlap);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator TargetRotation = bIsOpen ? OpenRotation : CloseRotation;
	float DoorSpeed = bIsOpen ? DoorOpenSpeed : DoorCloseSpeed;
	FRotator NewRotation = FMath::Lerp(DoorMesh->GetRelativeRotation(), TargetRotation, DeltaTime * DoorSpeed);
	DoorMesh->SetRelativeRotation(NewRotation);
}

void ADoor::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
	if (Player && bIsOpen)
	{
		bIsOpen = false;
		// Update player ui?
		FVector CheckPointLocation = CloseDoorTrigger->GetComponentLocation();
		CheckPointLocation.Z = Player->GetActorLocation().Z;;
		Player->HitCheckPoint(CheckPointLocation);
	}
}

void ADoor::SetDoorOpen(bool NewOpen)
{
	bIsOpen = NewOpen;
	UE_LOG(LogTemp, Warning, TEXT("Door %i %s"), DoorId, bIsOpen ? "Open" : "Closed");
}

