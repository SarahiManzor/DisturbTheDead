// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Engine/World.h"
#include "MainCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "TimerManager.h"
#include "Grave.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->bUseControllerDesiredRotation = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (Pawn)
	{
		Player = Cast<AMainCharacter>(Pawn);
	}
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	LockedZ = GetActorLocation().Z;

	if (SpawnParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticles, GetActorLocation(), FRotator(0.0f));
	}

	SetActorScale3D(FVector::ZeroVector);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemy::StartChase, 1.f, false, 1.f);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player)
	{
		if (bIsChasing)
		{
			FVector PlayerLocation = Player->GetActorLocation();
			FVector Target = PlayerLocation - GetActorLocation();
			Target.Normalize();
			//AddActorWorldOffset(Target * MovementSpeed * DeltaTime);
			AddMovementInput(Target, MovementSpeed);
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, LockedZ));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find player."));
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::StartChase()
{
	bIsChasing = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorScale3D(FVector::OneVector);
}

void AEnemy::Kill()
{
	bIsChasing = false;
	GetMovementComponent()->Velocity = FVector::ZeroVector;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorScale3D(FVector::ZeroVector);

	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticles, GetActorLocation(), FRotator(0.0f));
	}
}

void AEnemy::ResetGrave()
{
	if (HomeGrave)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reset grave"));
		HomeGrave->Bury();
	}
}

void AEnemy::SetHomeGrave(AGrave* Grave)
{
	HomeGrave = Grave;
}

