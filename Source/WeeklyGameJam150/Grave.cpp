// Fill out your copyright notice in the Description page of Project Settings.


#include "Grave.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGrave::AGrave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DirtHighlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DirtHighlightMesh"));
	DirtHighlightMesh->SetupAttachment(GetRootComponent());

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
	Health = BaseHealth;
	TargetRiseDelta = BaseRiseDelta;
}

// Called every frame
void AGrave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Health == 0 && TargetRiseDelta > 0)
	{
		float Rise = TargetRiseDelta * DeltaTime * RiseSpeed;
		TargetRiseDelta -= Rise;
		CoffinBaseMesh->SetRelativeLocation(CoffinBaseMesh->GetRelativeLocation() + FVector::UpVector * Rise);
		CoffinLidMesh->SetRelativeLocation(CoffinLidMesh->GetRelativeLocation() + FVector::UpVector * Rise);
		CoffinLidMesh->SetRelativeRotation(FRotator(0.f, 10.f * ((BaseRiseDelta - TargetRiseDelta) / BaseRiseDelta), 0.f));
	}

}

bool AGrave::Dig(AEnemy* &SpawnedEnemy)
{
	Health--;
	if (Health == 0)
	{
		SpawnedEnemy = SpawnGhost();
		SpawnedEnemy->SetHomeGrave(this);
		bSpawning = true;
		//InteractCollider->SetRelativeLocation(InteractCollider->GetRelativeLocation() + FVector::UpVector * 50.f);
		return bHasTreasure;
	}

	return false;
}

AEnemy* AGrave::SpawnGhost()
{
	if (Ghost)
	{
		AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(Ghost, GetSpawnLocation(), FRotator::ZeroRotator);
		return Enemy;
	}
	return nullptr;
}

void AGrave::Bury()
{
	Health = BaseHealth;
	//InteractCollider->SetRelativeLocation(InteractCollider->GetRelativeLocation() + FVector::UpVector * -50.f);
	CoffinBaseMesh->SetRelativeLocation(CoffinBaseMesh->GetRelativeLocation() - FVector::UpVector * (75.f - TargetRiseDelta));
	CoffinLidMesh->SetRelativeLocation(CoffinLidMesh->GetRelativeLocation() - FVector::UpVector * (75.f - TargetRiseDelta));
	CoffinLidMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	TargetRiseDelta = BaseRiseDelta;
	bSpawning = false;
}

void AGrave::SetHighlight_Implementation(bool NewHighlight)
{
	UE_LOG(LogTemp, Warning, TEXT("SetHighlightImplementation"));
}

FVector AGrave::GetSpawnLocation()
{
	return GetActorLocation() + GetActorRotation().RotateVector(GhostSpawnPoint);
}