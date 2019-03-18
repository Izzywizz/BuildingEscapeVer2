

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
}

void UOpenDoor::OpenDoor()
{
	//find the owner
	AActor *Owner = GetOwner();

	//set the rotation angle using rotator
	FRotator NewRotation = FRotator(0.0f, -90.0f, 0.0f);

	//Set the door angle using rotator
	Owner->SetActorRotation(NewRotation);

	//Print out current rotation
	FRotator Rotation = Owner->GetActorRotation();
	UE_LOG(LogTemp, Warning, TEXT("Door's Rotation is %s"), *Rotation.ToString());
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ... Poll the Trigger volume every frame means asking every frame to see if comeone enetesr
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatopens))
	{
		OpenDoor();
	}
	// if the actorThatOpens is in the volume
	// then we call Opendoor

}

