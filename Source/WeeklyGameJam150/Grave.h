// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grave.generated.h"

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
	UStaticMeshComponent* DirtHighlightMesh;
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

	UPROPERTY(EditAnywhere, Category = "Grave")
	int TreasureId = -1;

	int BaseHealth = 1;
	int Health;

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
	// also calls ghost spawn. Returns if grave had treasure
	bool Dig(AEnemy* &SpawnedEnemy);

	void Bury();

	UFUNCTION(BlueprintNativeEvent)
	void SetHighlight(bool NewHighlight);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsSpawned() { return bSpawning; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetTreasureId() { return TreasureId; }

	FVector GetSpawnLocation();

protected:
private:

};
