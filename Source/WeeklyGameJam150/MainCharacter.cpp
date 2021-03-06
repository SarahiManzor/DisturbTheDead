// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h" 
#include "Grave.h" 
#include "Enemy.h" 
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "WeeklyGameJam150GameModeBase.h"
#include "Engine/TriggerBox.h"
#include "Sound/SoundBase.h" 

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AMainCharacter::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AMainCharacter::ActorEndOverlap);

	CheckpointLocation = GetActorLocation();
	StartRotation = GetActorRotation();

	TreasuresCollected.Init(false, 3);
	NextInstruction(true);

	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(1.f, 0.f, 4.f, FLinearColor::Black);

	TotalDeaths = 0;
	PlayTime = 0;
	bGameComplete = false;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bGameComplete)
		PlayTime += DeltaTime;

	if (bCanSkip)
		bCanDig = false;
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Warning, TEXT("Setup player input"));
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAction(TEXT("LeftClick"), IE_Pressed, this, &AMainCharacter::SelectObject);
	PlayerInputComponent->BindAction(TEXT("SkipInstruction"), IE_Pressed, this, &AMainCharacter::NextInstruction);

}

void AMainCharacter::MoveForward(float AxisValue)
{
	if (AxisValue == 0.0f || !bIsAlive || bIsDigging) return;
	//UE_LOG(LogTemp, Warning, TEXT("Missing forward: %f"), AxisValue);

	if (!TopDownCameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing CAMERA!!"));
		return;
	}
	
	AddMovementInput(GetNormalizedXYProjectedLine(TopDownCameraComponent->GetForwardVector()), AxisValue * MaxMovementSpeed);
}

void AMainCharacter::MoveRight(float AxisValue)
{
	if (AxisValue == 0.0f || !bIsAlive || bIsDigging) return;
	//UE_LOG(LogTemp, Warning, TEXT("Missing forward: %f"), AxisValue);

	if (!TopDownCameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing CAMERA!!"));
		return;
	}

	AddMovementInput(GetNormalizedXYProjectedLine(TopDownCameraComponent->GetRightVector()), AxisValue * MaxMovementSpeed);
}

void AMainCharacter::SelectObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Selecting"));
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && bIsAlive && !bIsDigging && bCanDig)
	{
		bCanDig = false;
		UE_LOG(LogTemp, Warning, TEXT("Player controller found"));
		FVector2D MousePos;
		PC->GetMousePosition(MousePos.X, MousePos.Y);

		FHitResult Hit;		
		PC->GetHitResultAtScreenPosition(MousePos, ECC_GameTraceChannel2, true, Hit);
		if (Hit.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Hit.GetActor()->GetName());
			AGrave* Grave = Cast<AGrave>(Hit.GetActor()); // Todo: Make another class that handles generic interaction
			if (Grave)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit grave"));
			}
			if (Grave && DiggableGraves.Contains(Grave))
			{
				if (ShovelSound)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ShovelSound);
				}
				DiggableGraves.Remove(Grave);
				UE_LOG(LogTemp, Warning, TEXT("Is diggable"));

				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Grave->GetSpawnLocation());
				LookAtRotation.Pitch = 0.f;
				GetCharacterMovement()->Velocity = FVector::ZeroVector;
				SetActorRotation(LookAtRotation);
				bIsDigging = true;

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMainCharacter::FinishedDigging, .5f, false, .5f);

				AEnemy* SpawnedEnemy;
				if (Grave->Dig(SpawnedEnemy))
				{
					// Todo: Do something with the loot
					bGotTreasure = true;
					lastTreasureIndex = Grave->GetTreasureId();
					if (TreasuresCollected[Grave->GetTreasureId()])
					{
						bool bAllTrue = true;
						for (int32 i = 0; i < TreasuresCollected.Num(); i++)
						{
							if (!TreasuresCollected[i])
							{
								lastTreasureIndex += 1;
								TreasuresCollected[i] = true;
								bAllTrue = false;
								break;
							}
						}
						if (bAllTrue)
						{
							lastTreasureIndex += 1;
						}
					}
					else
					{
						TreasuresCollected[Grave->GetTreasureId()] = true;
					}
					NextInstruction(false);

					if (CorrectParticles)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CorrectParticles, GetActorLocation(), FRotator(0.0f));
					}
					if (CorrectSound)
					{
						UGameplayStatics::PlaySound2D(GetWorld(), CorrectSound, 0.75f);
					}
				}
				else if (IncorrectParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), IncorrectParticles, GetActorLocation(), FRotator(0.0f));
				}
				if (SpawnedEnemy)
				{
					Grave->SetHighlight(false);
					EnemyList.Add(SpawnedEnemy);
				}
			}
		}
		bCanDig = true;
	}
}

void AMainCharacter::NextInstruction()
{
	if (bIsAlive && bCanSkip) 
	{
		NextInstruction(bForceSkip);
		bForceSkip = false;
		bCanDig = true;
	}
}

void AMainCharacter::NextInstruction(bool Forced)
{
	bool bUpdated = false;
	AWeeklyGameJam150GameModeBase* GameMode = Cast<AWeeklyGameJam150GameModeBase>(UGameplayStatics::GetGameMode(this));
	if (TotalCollected >= 0 && lastTreasureIndex == TotalCollected || TotalCollected >= TreasuresCollected.Num() - 1)
	{
		if (TotalCollected < TreasuresCollected.Num() - 1 && TreasuresCollected[TotalCollected + 1])
		{
			GameMode->SkipNextInstruction();
		}
		else
		{
			GameMode->NextInstruction();
			bCanDig = true;
		}

		CurrentInstruction = GameMode->CurrentInstruction.Instruction;
		bCanSkip = GameMode->CurrentInstruction.bCanSkip;
		if (bCanSkip)
		{
			lastTreasureIndex += 1;
		}
		TotalCollected += 1;
		bUpdated = true;
	}

	if (!bUpdated && GameMode && (bCanSkip || Forced))
	{
		GameMode->NextInstruction(Forced);
		CurrentInstruction = GameMode->CurrentInstruction.Instruction;
		bCanSkip = GameMode->CurrentInstruction.bCanSkip;
		bUpdated = true;
	}

	if (!bUpdated && bGotTreasure)
	{
		FString LuckyString1 = "You got lucky with that one! Let's try to find the one I am referring to though. ";
		FString LuckyString2 = "Another lucky hit, but still not the one I was referring to! ";

		if (CurrentInstruction.Contains(LuckyString1))
		{
			CurrentInstruction.RemoveFromStart(LuckyString1);
			CurrentInstruction = LuckyString2 + CurrentInstruction;
		}
		else
		{
			CurrentInstruction = LuckyString1 + CurrentInstruction;
		}
	}
	
	bGotTreasure = false;
}

FString AMainCharacter::GetCurrentInstruction()
{
	return CurrentInstruction;
}

bool AMainCharacter::CanSkipInstruction()
{
	return bCanSkip;
}


void AMainCharacter::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Begin overlapped: %s"), *OverlappedActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Begin other: %s"), *OtherActor->GetName());

	AGrave* Grave = Cast<AGrave>(OtherActor);
	if (Grave)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grave found: %s"), *Grave->GetName());
		DiggableGraves.Add(Grave);
		if (!Grave->IsSpawned())
			Grave->SetHighlight(true);
		return;
	}

	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy && bIsAlive)
	{
		bIsAlive = false;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0.f, 1.f, 1.5f, FLinearColor::Black, false, true);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMainCharacter::StartRestart, 2.f, false, 2.f);
		if (DeathSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), DeathSound);
		}
	}

	ATriggerBox* TriggerBox = Cast<ATriggerBox>(OtherActor);
	if (TriggerBox && TriggerBox->GetName().Contains(TEXT("Complete")))// box
	{
		bGameComplete = true;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0.f, 1.f, 1.5f, FLinearColor::Black, false, true);
		GetController()->SetIgnoreLookInput(true);
		GetController()->SetIgnoreMoveInput(true);
	}
}

void AMainCharacter::StartRestart()
{
	SetActorLocation(CheckpointLocation);
	SetActorRotation(StartRotation);
	ClearProgress();
	bIsAlive = true;
	bIsDigging = false;
	bCanDig = true;
	TotalDeaths += 1;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMainCharacter::FadeCameraIn, 1.f, false, 1.f);
}

void AMainCharacter::FadeCameraIn()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(1.f, 0.f, 1.5f, FLinearColor::Black);
}

void AMainCharacter::HitCheckPoint(FVector CheckPoint)
{
	CheckpointLocation = CheckPoint;
	AWeeklyGameJam150GameModeBase* GameMode = Cast<AWeeklyGameJam150GameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->SetResetIndex();
		bCanSkip = true;
		bForceSkip = true;
		bCanDig = false;
		GameMode->NextLevel();
	}
	ResetForNextLevel();
}

void AMainCharacter::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("End overlapped: %s"), *OverlappedActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("End other!!: %s"), *OtherActor->GetName());

	AGrave* Grave = Cast<AGrave>(OtherActor);
	if (Grave)
	{
		if (DiggableGraves.Contains(Grave))
		{
			Grave->SetHighlight(false);
			DiggableGraves.Remove(Grave);
		}
	}
}

FVector AMainCharacter::GetNormalizedXYProjectedLine(FVector InputVector)
{
	// Get bigger vector from x and y (not z)
	FVector OutputVector = InputVector.X > InputVector.Y ? FVector(InputVector.X, 0.f, 0.f) : FVector(0.f, InputVector.Y, 0.f);
	OutputVector.Normalize();
	return OutputVector;
}

void AMainCharacter::ResetForNextLevel()
{
	for (AEnemy* Enemy : EnemyList)
	{
		Enemy->Kill();
	}
	EnemyList.Empty();

	lastTreasureIndex = -1;
	TotalCollected = 0;
	TreasuresCollected.Empty();
	TreasuresCollected.Init(false, 3);
}

void AMainCharacter::ClearProgress()
{
	for (AEnemy* Enemy : EnemyList)
	{
		Enemy->ResetGrave();
		Enemy->Destroy();
	}
	EnemyList.Empty();

	AWeeklyGameJam150GameModeBase* GameMode = Cast<AWeeklyGameJam150GameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->FailLevel();
		GameMode->NextInstruction(true);
		CurrentInstruction = GameMode->CurrentInstruction.Instruction;
		bCanSkip = GameMode->CurrentInstruction.bCanSkip;
	}

	lastTreasureIndex = -1;
	TotalCollected = 0;
	TreasuresCollected.Empty();
	TreasuresCollected.Init(false, 3);
}

void AMainCharacter::FinishedDigging()
{
	bIsDigging = false;
}
