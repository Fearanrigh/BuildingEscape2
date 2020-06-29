// Copyright Sean Barton 2020

#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for attached Input Component
void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input component found on %s"), *(GetOwner()->GetName()));
		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Input component not found on %s!"), *(GetOwner()->GetName()));
	}
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle) {
		// Physics handle is found
	} else {
		UE_LOG(LogTemp, Error, TEXT("Physics handle component not found on %s!"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab() {
	/// 
	UE_LOG(LogTemp, Error, TEXT("Grab pressed"));

	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	GetFirstPhysicsBodyInReach();

	/// If we hit something then attach a physics handle
	// TODO attach physics handle
}

void UGrabber::Release() {
	///
	UE_LOG(LogTemp, Error, TEXT("Grab released"));
	// TODO release physics handle
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
		// move the object that we're holding
	
}

void UGrabber::DrawDebugLineHelper(FVector PlayerViewPointLocation, FVector LineTraceEnd) {
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0,
		10.0f
	);
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	/// Get the player viewpoint this tick.
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation);
	
	/// Create the line trace end point
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	/// Draw a red trace in the world to visualize.
	// DrawDebugLineHelper(PlayerViewPointLocation, LineTraceEnd);

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(
		FName(TEXT("")),
		false,
		GetOwner()
	);

	/// Ray-cast (AKA Line-trace) out to reach distance.
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// See what we hit.
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}

