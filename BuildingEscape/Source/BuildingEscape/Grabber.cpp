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

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle) {
		// Physics handle is found
	} else {
		UE_LOG(LogTemp, Error, TEXT("Physics handle component not found on %s!"), *(GetOwner()->GetName()));
	}

	/// Look for attached Input Component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input component found on %s"), *(GetOwner()->GetName()));
		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Input component not found on %s!"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab() {
	// 
	UE_LOG(LogTemp, Error, TEXT("Grab pressed"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get the player viewpoint this tick.
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation);
	
	/// Draw a red trace in the world to visualize.
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

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
	
}

