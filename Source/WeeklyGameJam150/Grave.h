// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grave.generated.h"

class USkeletalMeshComponent;
class AEnemy;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class WEEKLYGAMEJAM150_API AGrave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ----------Variables----------
public:
protected:
private:
	UPROPERTY(VisibleAnywhere, Category = "Grave")
	USkeletalMeshComponent* DirtMesh;

	UPROPERTY(VisibleAnywhere, Category = "Grave")
	UStaticMeshComponent* CoffinBaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Grave")
		UStaticMeshComponent* CoffinLidMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Grave")
	TSubclassOf<AEnemy> Ghost;

	UPROPERTY(VisibleAnywhere, Category = "Grave")
	UBoxComponent* InteractCollider;

	UPROPERTY(VisibleAnywhere, Category = "Grave")
	UBoxComponent* RangeCollider;

	UPROPERTY(EditAnywhere, Category = "Grave", meta = (MakeEditWidget = "true"))
	FVector GhostSpawnPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Grave")
	float Value = 0.f;

	float Health = 1.f;

	// ----------Custom Functions----------
public:
	// Reduces health of grave and returns value if health hits zero
	// also calls ghost spawn
	bool Dig(float &OutValue);

	// Spawns ghost at GhostSpawnPoint
	void SpawnGhost();
protected:
private:

};
