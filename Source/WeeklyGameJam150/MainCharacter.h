// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class AGrave;
class AEnemy;

UCLASS()
class WEEKLYGAMEJAM150_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// ----------Variables----------
public:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

protected:
private:
	FRotator StartRotation;

	// Movement speed in m/sec
	UPROPERTY(EditAnywhere)
	float MaxMovementSpeed = 100.f;

	TArray<AGrave*> DiggableGraves;

	int TotalCollected = 0;
	bool bIsAlive = true;
	bool bIsDigging = false;

	FVector CheckpointLocation = FVector::ZeroVector;

	TArray<AEnemy*> EnemyList;

	// ----------Custom Functions----------
public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetTotalCollected() { return TotalCollected; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsAlive() { return bIsAlive; }

	void ResetForNextLevel();

	void HitCheckPoint(FVector CheckPoint);

	UFUNCTION()
	FORCEINLINE bool GetIsDigging() { return bIsDigging; }

	UFUNCTION()
	FORCEINLINE bool GetIsAlive() { return bIsAlive; }

protected:
private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void SelectObject();
	FVector GetNormalizedXYProjectedLine(FVector InputVector);

	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void StartRestart();

	UFUNCTION()
	void FadeCameraIn();

	UFUNCTION()
	void ClearProgress();

	UFUNCTION()
	void FinishedDigging();
};
