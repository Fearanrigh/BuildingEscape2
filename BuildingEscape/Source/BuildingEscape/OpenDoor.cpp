// Copyright Sean Barton 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if(!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate!"), *(GetOwner()->GetName()));
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume every frame
	if(GetTotalMassOfActorsOnPlate() > 30.0f) { // TODO make into a parameter
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door.
	if(GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor() {
	if(!Owner) { return; }
	FRotator ObjectRotation = Owner->GetActorRotation();
	ObjectRotation.Yaw = OpenAngle;
	// Owner->SetActorRotation(ObjectRotation);
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor() {
	if(!Owner) { return; }
	FRotator ObjectRotation = Owner->GetActorRotation();
	ObjectRotation.Yaw = 0.0f;
	Owner->SetActorRotation(ObjectRotation);
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.0f;

	// Find all the overlapping actors.
	TArray<AActor*> OverlappingActors;
	if(!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through the actors, adding their masses.
	for(const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Error, TEXT("%s on pressure plate"), *Actor->GetName())
	}

	return TotalMass;
}