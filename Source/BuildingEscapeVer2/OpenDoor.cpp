

#include "OpenDoor.h"
#include "GameFramework/Actor.h" //getOwner autocomplete
#include "GameFramework/PlayerController.h"
#include "Engine/World.h" //GetWorld autocompletes
#include "GameFramework/Pawn.h" //GetPawn 



class UWorld;

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

	// ...We've searched top down through the world for the playercontroller and the pawn attached to it
	ActorThatopens = GetWorld()->GetFirstPlayerController()->GetPawn(); //remember that although it returns a PAWM, a PAWN IS-A inherits from an Actor
	
																		//find the owner
	Owner = GetOwner();

}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));

	//Print out current rotation
	FRotator Rotation = Owner->GetActorRotation();
	UE_LOG(LogTemp, Warning, TEXT("Door's Rotation is %s"), *Rotation.ToString());
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}



// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ... Poll the Trigger volume every frame means asking every frame to see if someone can enter
	// if the actorThatOpens is in the volume
	// then we call Opendoor
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatopens))
	{
		OpenDoor();
		//lets take a note the last time the door was open
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//check if it's time to close the door
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

