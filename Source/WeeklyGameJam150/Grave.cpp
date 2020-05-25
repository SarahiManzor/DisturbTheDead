// Fill out your copyright notice in the Description page of Project Settings.


#include "Grave.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGrave::AGrave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DirtMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DirtMesh"));
	DirtMesh->SetupAttachment(GetRootComponent());

	CoffinBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoffinBaseMesh"));
	CoffinBaseMesh->SetupAttachment(GetRootComponent());

	CoffinLidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoffinLidMesh"));
	CoffinLidMesh->SetupAttachment(GetRootComponent());

	InteractCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollider"));
	InteractCollider->SetupAttachment(GetRootComponent());
	InteractCollider->SetCollisionObjectType(ECC_GameTraceChannel2);
	InteractCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractCollider->SetRelativeLocation(FVector(0.f, 150.f, -32.f));
	InteractCollider->SetWorldScale3D(FVector(1.25f, 4.25f, 1.f));

	RangeCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("RangeCollider"));
	RangeCollider->SetupAttachment(GetRootComponent());
	RangeCollider->SetCollisionObjectType(ECC_WorldStatic);
	RangeCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RangeCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RangeCollider->SetRelativeLocation(FVector(0.f, 150.f, 0.f));
	RangeCollider->SetWorldScale3D(FVector(4.f, 6.f, 3.f));

	GhostSpawnPoint = FVector(0.f, 130.f, 90.f);
}

// Called when the game starts or when spawned
void AGrave::BeginPlay()
{
	Super::BeginPlay();

	if (GhostSpawnPoint == FVector::ZeroVector)
	{
		GhostSpawnPoint = GetActorLocation();
	}	
}

// Called every frame
void AGrave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGrave::Dig(AEnemy* &SpawnedEnemy)
{
	Health--;
	if (Health == 0)
	{
		SpawnedEnemy = SpawnGhost();
		bSpawning = true;
		InteractCollider->SetRelativeLocation(InteractCollider->GetRelativeLocation() + FVector::UpVector * InteractCollider->GetRelativeLocation() * -2.f);
		CoffinBaseMesh->SetRelativeLocation(CoffinBaseMesh->GetRelativeLocation() + FVector::UpVector * 75.f);
		CoffinLidMesh->SetRelativeLocation(CoffinLidMesh->GetRelativeLocation() + FVector::UpVector * 75.f);
		CoffinLidMesh->SetRelativeRotation(FRotator(0.f, 10.f, 0.f));
		return bHasTreasure;
	}

	return false;
}

AEnemy* AGrave::SpawnGhost()
{
	if (Ghost)
	{
		AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(Ghost, GetActorLocation() + GetActorRotation().RotateVector(GhostSpawnPoint), FRotator::ZeroRotator);
		return Enemy;
	}
	return nullptr;
}

