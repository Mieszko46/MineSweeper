#include "Grabber.h"

#define OUT

// Sets default values
UGrabber::UGrabber():
	FocusedActor(nullptr),
	bDebugFlag(true), 
	DebugIndex(-1)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewLocation = GetPlayerLocation();
	FRotator PlayerViewRotation = GetPlayerRotation();

	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * m_Reach;

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	AActor* ActorHit = HitResult.GetActor();
	
	if (ActorHit) {
		HighlightObject(ActorHit);
	}
	else {
		if (FocusedActor)
			Cast<ACubePackage>(FocusedActor)->SetIsActorFocued(false);
			FocusedActor = nullptr;
	}

	//if(!PhysicsHandle)
	//	return;

	//if (PhysicsHandle->GrabbedComponent) {
	//	PhysicsHandle->SetTargetLocation(GetPlayerReach());
	//}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("No physics handle component included to object: %s !"), *GetOwner()->GetName());
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		if (PhysicsHandle)
			return;
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayerReach());
	};
}

void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		if (!PhysicsHandle)
			return;
		PhysicsHandle->ReleaseComponent();
	}
}

FVector UGrabber::GetPlayerLocation() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	return PlayerViewLocation;
}

FRotator UGrabber::GetPlayerRotation() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	return PlayerViewRotation;
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewLocation = GetPlayerLocation();
	FRotator PlayerViewRotation = GetPlayerRotation();

	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * m_Reach;

	//UE_LOG(LogTemp, Warning, TEXT("Actor location: %s"), *LineTraceEnd.ToString());

	DrawDebugLine(
		GetWorld(),
		PlayerViewLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0,
		2.0f
	);

	return LineTraceEnd;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(),
		TraceParams
	);

	return Hit;
}

void UGrabber::HighlightObject(AActor* ActorHit)
{
	if (ActorHit->IsA<ACubePackage>()) 
	{
		if (FocusedActor != ActorHit) 
		{
			// unset highlight of passed object
			if (FocusedActor != nullptr) 
			{
				Cast<ACubePackage>(FocusedActor)->SetIsActorFocued(false);
			}

			FocusedActor = ActorHit;
			ACubePackage* HittedPackage = Cast<ACubePackage>(FocusedActor);

			AActorSpawner* Spawner = Cast<AActorSpawner>(HittedPackage->GetPackageSpawner());
			if (Spawner->CheckIfPackageCanBePicked(HittedPackage->GetPackageIndex()))
			{
				HittedPackage->SetIsActorFocued(true);
			}

			UE_LOG(LogTemp, Warning, TEXT("Hited package: %s"), *(HittedPackage->GetName()));
			UE_LOG(LogTemp, Warning, TEXT("Hited package index: %d"), HittedPackage->GetPackageIndex());
			UE_LOG(
				LogTemp, Warning, TEXT("Does package can be picked: %s"),
				(Spawner->CheckIfPackageCanBePicked(HittedPackage->GetPackageIndex()) ? TEXT("TRUE") : TEXT("false"))
			);
			UE_LOG(
				LogTemp, Warning, TEXT("Is it last in Z: %s"),
				((HittedPackage->GetIsLastInZ_Axis()) ? TEXT("TRUE") : TEXT("false"))
			);
			//UE_LOG(
			//	LogTemp, Warning, TEXT("Does package contain mine: %s"),
			//	((HittedPackage->IsItMine()) ? TEXT("TRUE") : TEXT("false"))
			//);
		}
	}
}
