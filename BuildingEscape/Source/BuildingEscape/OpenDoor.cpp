// Copyright Sean Barton 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume every frame
	// If the actor that opens is in the volume, then we open the door.
	if(PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor();
	}
}

void UOpenDoor::OpenDoor() {
	AActor* Owner = GetOwner();
	FRotator ObjectRotation = Owner->GetActorRotation();

	UE_LOG(LogTemp, Warning, TEXT("Rotation is %.3f"), ObjectRotation.Yaw)

	ObjectRotation.Yaw = 60.0f;
	Owner->SetActorRotation(ObjectRotation);
}
