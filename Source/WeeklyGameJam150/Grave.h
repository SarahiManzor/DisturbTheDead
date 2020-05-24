// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grave.generated.h"

class USkeletalMeshComponent;
class AEnemy;

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
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)	
	TSubclassOf<AEnemy> Ghost;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	FVector GhostSpawnPoint = FVector::ZeroVector;

	// ----------Custom Functions----------
public:
protected:
private:

};
