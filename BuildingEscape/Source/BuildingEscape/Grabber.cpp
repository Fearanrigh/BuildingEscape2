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
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Input component not found on %s!"), *(GetOwner()->GetName()));
	}
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Physics handle component not found on %s!"), *(GetOwner()->GetName()));
	}	
}

void UGrabber::Grab() {
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if(ActorHit != nullptr) {
		// TODO attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release() {
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetLineTraceEnd();

	// if the physics handle is attached
	if(PhysicsHandle->GrabbedComponent) {
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::SetActorLocationAndRotation() {
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);
}

/// Updates the player view location/rotation and line trace end.
void UGrabber::SetLineTraceEnd() {
	/// Gets the player position and rotation and updates the variables.
	SetActorLocationAndRotation();
	
	/// Create the line trace end point
	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

void UGrabber::DrawDebugLineHelper() {
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

	SetLineTraceEnd();

	/// Draw a red trace in the world to visualize.
	// DrawDebugLineHelper();

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(
		FName(TEXT("")),
		false,
		GetOwner()
	);

	/// Ray-cast (AKA Line-trace) out to reach distance.
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return HitResult;
}