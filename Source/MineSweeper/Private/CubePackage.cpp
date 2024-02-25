// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePackage.h"

// Sets default values
ACubePackage::ACubePackage():
	bIsMine(false),
	bWasPicked(false),
	IndexInArray(-1),
	nearMines(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);
}

// Called when the game starts or when spawned
void ACubePackage::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACubePackage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubePackage::SetPackageIndex(uint32 index)
{
	IndexInArray = index;
}

uint32 ACubePackage::GetPackageIndex() const
{
	return IndexInArray;
}
