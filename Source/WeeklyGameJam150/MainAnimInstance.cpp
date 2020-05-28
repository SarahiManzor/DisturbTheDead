// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "MainCharacter.h"
#include "Math/UnrealMathUtility.h" 

void UMainAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Main = Cast<AMainCharacter>(Pawn);
		}
	}

}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (!Pawn)
		Pawn = TryGetPawnOwner();

	if (!Pawn) return;

	FVector Speed = Pawn->GetVelocity();
	FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);
	MovementSpeed = LateralSpeed.Size();

	if (Main == nullptr)
	{
		Main = Cast<AMainCharacter>(Pawn);
	}

	if (Main)
	{
		bIsDigging = Main->GetIsDigging();
		bIsAlive = Main->GetIsAlive();
	}
}

USoundBase* UMainAnimInstance::GetRandomStepSound()
{
	int32 RandomIndex = FMath::RandRange(0, StepSounds.Num() - 1);
	if (RandomIndex > StepSounds.Num() - 1 || RandomIndex < 0) return nullptr;
	return StepSounds[RandomIndex];
}
