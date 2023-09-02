// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSpawner.h"

// Sets default values
AActorSpawner::AActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	FTransform SpawnerTransform = SpawnerTargetPoint
		? SpawnerTargetPoint->GetActorTransform()
		: FTransform::Identity;

	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnerTransform, SpawnParams);
	
}

// Called every frame
void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

