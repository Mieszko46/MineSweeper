// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubePackage.h"
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

	void PrintArray();
	bool CheckIfPackageCanBePicked(int32 index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	#pragma region Board methods
	void CreateBoardOnSpawnPoint();
	void RandomizeBoardDimensions();
	void PlaceThePackages();
	#pragma endregion

	#pragma region Packages methods
	// get package from an array of all packages 
	// @param x - package position in x-axis
	// @param y - package position in y-axis
	// @param z - package position in z-axis
	AActor* GetActorByIndex(int32 index) const;

	void CalculateCoordinatesFromIndex(uint32 index, uint32& out_x, uint32& out_y, uint32& out_z);
	uint32 CalculateIndexFromCoordinates(uint32 x, uint32 y, uint32 z) const;

	void SetTotalNumberOfPackages(uint32 packages);
	#pragma endregion

	#pragma region Mines methods
	void SetTotalNumberOfMines(uint32 mines);
	void RandomizeMinesPlacement();
	#pragma endregion

	#pragma region ATTRIBUTES
	// Actor that will be spawned
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AActor> ActorToSpawn;

	// Position of selected spawn point
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSoftObjectPtr<ASpawnerTarget> SpawnerTargetPoint;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		uint32 PackageSize;


	// 3d Array of all packages - to mo¿emy daæ do jakiegoœ innego obiektu, da³bym to do Spawnera, bo jest jeden i z tamt¹d byœmy
	// odwo³ywali siê do poszczególnych obiektów, dziêki czemu mamy lepsz¹ optymalizacjê, mniej pamiêci bêdziemy zajmowaæ
	TArray<AActor*> AllPackages;

	// Board dimension
	uint32 X_Width, Y_Height, Z_Deep;

	uint32 TotalNumberOfPackages;
	uint32 TotalNumberOfMines;

	const float FactorOfBoardComplexity = 1.5f;
	#pragma endregion

};
