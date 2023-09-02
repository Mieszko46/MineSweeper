// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerTarget.h"
#include "ActorSpawner.generated.h"

UCLASS()
class MINESWEEPER_API AActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorSpawner();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	// Sets actor that will be spawned
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AActor> ActorToSpawn;
	// Sets position of selected spawn point
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSoftObjectPtr<ASpawnerTarget> SpawnerTargetPoint;

};
