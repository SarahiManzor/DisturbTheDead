// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
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
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
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

FVector AMainCharacter::GetNormalizedXYProjectedLine(FVector InputVector)
{
	// Get bigger vector from x and y (not z)
	FVector OutputVector = InputVector.X > InputVector.Y ? FVector(InputVector.X, 0.f, 0.f) : FVector(0.f, InputVector.Y, 0.f);
	OutputVector.Normalize();
	return OutputVector;
}

