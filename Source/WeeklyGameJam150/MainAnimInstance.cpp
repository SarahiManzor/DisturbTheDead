// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "MainCharacter.h"

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
}
