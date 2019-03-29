

#include "OpenDoor.h"
#include "GameFramework/Actor.h" //getOwner autocomplete
#include "GameFramework/PlayerController.h"
#include "Engine/World.h" //GetWorld autocompletes
#include "GameFramework/Pawn.h" //GetPawn 
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h" //GetMass() for Actor

// Used as reminder that the parameter we are using is a reference to the actual parameter and thus can be changed,
//remember the #define OUt does nothing, it replaces it with nothing!
#define OUT

class UWorld;

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
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

	if (GetTotalMassOfActorsOnPlate() > 30.0f) // TODO make into parameter that is exposesd in the editor.
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

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	// Find overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// After this point we should get all the overlapping Acotrs on the PressurePlate ie ATriggerVolume

	// Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		float Mass = Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		TotalMass += Mass;
		UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s Mass: %f"), *Actor->GetName(), Mass);
	}

	return TotalMass;
}

