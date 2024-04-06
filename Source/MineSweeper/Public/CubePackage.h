// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AActorSpawner;
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "CubePackage.generated.h"

UCLASS()
class MINESWEEPER_API ACubePackage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubePackage();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPackageIndex(uint32 index);
	uint32 GetPackageIndex() const;

	void SetPackageSpawner(AActor* Spawner);
	AActor* GetPackageSpawner() const;

	#pragma region Mines methods
	void SetIsMineToTrue();
	bool IsItMine() const;
	void SetNumberOfNearMines(uint32 nearMines);
	uint32 GetNumberOfNearMines() const;
	#pragma endregion

	void SetIsActorFocued(bool isFocused);
	bool GetIsActorFocued() const;

	void SetIsLastInZ_Axis(bool isLast);
	bool GetIsLastInZ_Axis() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMesh;

	AActor* PackageSpawner;

	// Does ACTOR HAVE a MINE
	bool bIsMine;

	// Has ACTOR been already PICKED
	bool bWasPicked;

	// Does player look on this actor
	bool IsActorFocued;

	// Index in array
	uint32 IndexInArray;

	// Is it last element in Z axis?
	bool isLastIn_Z;

	// How many MINES are NEAR
	uint32 NumberOfNearMines;
};
