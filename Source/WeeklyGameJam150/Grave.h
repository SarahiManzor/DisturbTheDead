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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grave")
	USkeletalMeshComponent* DirtMesh;
protected:
private:

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
	bool bHasTreasure = false;

	float BaseHealth = 1.f;
	float Health;

	float BaseRiseDelta = 75.f;
	float TargetRiseDelta;

	UPROPERTY(EditAnywhere)
	float RiseSpeed = 1.f;

	bool bSpawning = false;

	// Spawns ghost at GhostSpawnPoint
	AEnemy* SpawnGhost();

	// ----------Custom Functions----------
public:
	// Reduces health of grave and returns value if health hits zero
	// also calls ghost spawn
	bool Dig(AEnemy* &SpawnedEnemy);

	void Bury();

	UFUNCTION(BlueprintNativeEvent)
	void SetHighlight(bool NewHighlight);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsSpawned() { return bSpawning; }

	FVector GetSpawnLocation();

protected:
private:

};
