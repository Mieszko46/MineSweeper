// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorSpawner.h"

//TODO: przed RandomizeBoardDimensions() trzeba podaæ od gracza liczbê paczek

AActorSpawner::AActorSpawner(): 
	X_Width(1),
	Y_Height(1),
	Z_Deep(1),
	PackageSize(100),
	TotalNumberOfPackages(10),
	TotalNumberOfMines(5)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();
	CreateBoardOnSpawnPoint();
}

void AActorSpawner::CreateBoardOnSpawnPoint()
{
	RandomizeBoardDimensions();

	// init board with nullptr
	//AllPackages.Init(AActor(), X_Width * Y_Height * Z_Deep);
	UE_LOG(LogTemp, Error, TEXT("Array size: %d"), X_Width * Y_Height * Z_Deep);
	AllPackages.Reserve(X_Width * Y_Height * Z_Deep);
	AllPackages.SetNumZeroed(X_Width * Y_Height * Z_Deep);

	PlaceThePackages();
}

void AActorSpawner::PrintArray()
{
	for (int32 i = 0; i < AllPackages.Num(); i++)
	{
		auto Package = AllPackages[i];
		if (Package != nullptr)
			UE_LOG(LogTemp, Error, TEXT("array elem on pos %d is: %d"), i, Package);
		
	}
}

void AActorSpawner::RandomizeBoardDimensions()
{
	while (true)
	{
		uint32 dimensionNumber = FMath::RandHelper(3);

		if (dimensionNumber == 0)
			X_Width += 1;
		else if (dimensionNumber == 1) 
			Y_Height += 1;
		else
			Z_Deep += 1;

		if (X_Width * Y_Height * Z_Deep >= TotalNumberOfPackages * FactorOfBoardComplexity)
			break;
	}
}

void AActorSpawner::PlaceThePackages()
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, AllPackages[1].GetName());

	UE_LOG(LogTemp, Error, TEXT("BOARD params: %d, %d, %d"), X_Width, Y_Height, Z_Deep);

	FActorSpawnParameters SpawnParams;

	FTransform SpawnerTransform = SpawnerTargetPoint
		? SpawnerTargetPoint->GetActorTransform()
		: FTransform::Identity;

	uint32 CurrentPlacedPackages = 0;
	while (CurrentPlacedPackages < TotalNumberOfPackages)
	{
		FActorSpawnParameters temp = SpawnParams;

		FTransform InitTransform = SpawnerTransform;
		// We take random position in 2d board and then go in deep 
		uint32 X_RandomPosition = FMath::RandHelper(X_Width);
		uint32 Y_RandomPosition = FMath::RandHelper(Y_Height);

		for (uint32 Z_Position = 0; Z_Position < Z_Deep; Z_Position++)
		{
			int32 index = X_RandomPosition * Y_Height * Z_Deep + Y_RandomPosition * Z_Deep + Z_Position;
			//check if position is empty
			if (AllPackages[index] == nullptr)
			{
				// Spawn object
				FVector offset = FVector(PackageSize * X_RandomPosition, PackageSize * Y_RandomPosition, PackageSize * Z_Position);
				InitTransform.AddToTranslation(offset);
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, InitTransform, temp);

				// Insert package to array
				AllPackages[index] = SpawnedActor;

				CurrentPlacedPackages += 1;

				//Set package index
				ACubePackage* Package = Cast<ACubePackage>(SpawnedActor);
				Package->SetPackageIndex(index);

				UE_LOG(LogTemp, Error, TEXT("Actor: %s | Package index: %d | coords: x=%d y=%d z=%d"), 
					*(SpawnedActor->GetName()), 
					Package->GetPackageIndex(),
					X_RandomPosition,
					Y_RandomPosition,
					Z_Position
				);

				// To break from the loop
				Z_Position = Z_Deep;
			}
		}
	}
}

void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AActor* AActorSpawner::GetActorByIndex(int32 index)
{
	auto Package = AllPackages[index];
	return Package;
}

void AActorSpawner::CalculateCoordinatesFromIndex(uint32 index, uint32& out_x, uint32& out_y, uint32& out_z)
{
	out_z = index % Z_Deep;
	index /= Z_Deep;
	out_y = index % Y_Height;
	index /= Y_Height;
	out_x = index;
}

void AActorSpawner::SetTotalNumberOfPackages(uint32 packages)
{
	TotalNumberOfPackages = packages;
}

void AActorSpawner::SetTotalNumberOfMines(uint32 mines)
{
	TotalNumberOfMines = mines;
}
