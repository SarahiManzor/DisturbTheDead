// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DisturbTheDeadCharacter.generated.h"

UCLASS(Blueprintable)
class ADisturbTheDeadCharacter : public ACharacter
{
	GENERATED_BODY()

// ----------Variables----------
public:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

protected:
private:
	// Movement speed in m/sec
	UPROPERTY(EditAnywhere)
	float MaxMovementSpeed = 100.f;

// ----------Base Functions----------
public:
	// Sets default values for this pawn's properties
	ADisturbTheDeadCharacter();

protected:
private:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// ----------Custom Functions----------
public:
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

protected:
private:
	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION()
	FVector GetNormalizedXYProjectedLine(FVector InputVector);
};

