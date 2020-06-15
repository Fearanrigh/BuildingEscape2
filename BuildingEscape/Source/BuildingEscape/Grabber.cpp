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
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the player viewpoint this tick.
	FVector viewLocation;
	FRotator viewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT viewLocation, 
		OUT viewRotation);
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *viewRotation.ToString());
	
	// Draw a red trace in the world to visualize.
	FVector LineTraceEnd = viewLocation + viewRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		viewLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0,
		10.0f
	);

	// Ray-cast out to reach distance.

	// See what we hit.
}

