// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UStaticMeshComponent;

UCLASS()
class WEEKLYGAMEJAM150_API ADoor : public AActor
{
	GENERATED_BODY()
	
		// ----------Base Functions----------
public:
	// Sets default values for this pawn's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ----------Variables----------
public:
protected:
private:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorFrameMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorMesh;

	// ----------Custom Functions----------
public:
	void OpenDoor();
protected:
private:
	UPROPERTY(EditAnywhere, category = "Door Settings")
	int DoorID = 0;

	UPROPERTY(EditAnywhere, category = "Door Settings")
	int TreasureToOpen = 4;
	
	UPROPERTY(EditAnywhere, category = "Door Settings")
	bool bIsOpen;
	
	UPROPERTY(EditAnywhere, category = "Door Settings")
	FRotator OpenRotation;

	UPROPERTY(EditAnywhere, category = "Door Settings")
	float DoorSpeed = 10.f;

	UPROPERTY(EditAnywhere, category = "Door Settings")
	FRotator CloseRotation;

};
