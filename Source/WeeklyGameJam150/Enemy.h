// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AMainCharacter;

UCLASS()
class WEEKLYGAMEJAM150_API AEnemy : public ACharacter
{
	GENERATED_BODY()

// ----------Base Functions----------
public:
	// Sets default values for this character's properties
	AEnemy();

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
protected:
private:
	AMainCharacter* Player;

	UPROPERTY(EditAnywhere)
	float MovementSpeed = 50.f;

	float LockedZ;

	bool bIsChasing = false;

// ----------Custom Functions----------
public:
	void Kill();
protected:
private:
	void StartChase();

};
