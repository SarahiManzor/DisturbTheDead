// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h" 
#include "Grave.h" 
#include "Enemy.h" 

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
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(-1000, 300, 20)));
	//UE_LOG(LogTemp, Warning, TEXT("Ticking"));
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Warning, TEXT("Setup player input"));
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAction(TEXT("LeftClick"), IE_Pressed, this, &AMainCharacter::SelectObject);

}

void AMainCharacter::MoveForward(float AxisValue)
{
	if (AxisValue == 0.0f) return;

	if (!TopDownCameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing CAMERA!!"));
		return;
	}
	
	AddMovementInput(GetNormalizedXYProjectedLine(TopDownCameraComponent->GetForwardVector()), AxisValue * MaxMovementSpeed);
}

void AMainCharacter::MoveRight(float AxisValue)
{
	if (AxisValue == 0.0f) return;

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
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player controller found"));
		FVector2D MousePos;
		PC->GetMousePosition(MousePos.X, MousePos.Y);

		/*int32 ViewPortSizeX;
		int32 ViewPortSizeY;
		PC->GetViewportSize(ViewPortSizeX, ViewPortSizeY);
		FVector2D ScreenLocation(ViewPortSizeX * MousePos.X, ViewPortSizeY * MousePos.Y);

		FVector WorldLocation;
		FVector LookDirection;
		PC->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, LookDirection);*/

		FHitResult Hit;
		//GetWorld()->LineTraceSingleByChannel(Hit, TopDownCameraComponent->GetComponentLocation(), TopDownCameraComponent->GetComponentLocation() + LookDirection * 1000.f, ECC_GameTraceChannel2);
		
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
				UE_LOG(LogTemp, Warning, TEXT("Is diggable"));

				if (Grave->Dig())
				{
					// Todo: Do something with the loot
					TotalCollected += 1;
				}
			}
		}
	}
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
		return;
	}

	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		bIsAlive = false;
	}
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

