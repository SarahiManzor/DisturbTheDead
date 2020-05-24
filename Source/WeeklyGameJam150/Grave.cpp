// Fill out your copyright notice in the Description page of Project Settings.


#include "Grave.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Enemy.h"

// Sets default values
AGrave::AGrave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AGrave::BeginPlay()
{
	Super::BeginPlay();

	if (GhostSpawnPoint == FVector::ZeroVector)
	{
		GhostSpawnPoint = GetActorLocation();
	}

	if (Ghost)
	{
		AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(Ghost, GetActorLocation() + GetActorRotation().RotateVector(GhostSpawnPoint), FRotator::ZeroRotator);
	}
	
}

// Called every frame
void AGrave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

