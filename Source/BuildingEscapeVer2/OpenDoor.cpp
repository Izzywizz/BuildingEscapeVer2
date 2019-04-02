

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

	if (PressurePlate == nullptr)
	{
		FString name = GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("PressurePlate Not found within %s "), *name);
	}
	//find the owner
	Owner = GetOwner();

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ... Poll the Trigger volume every frame means asking every frame to see if someone can enter

	if (GetTotalMassOfActorsOnPlate() > TriggerMass) // TODO make into parameter that is exposesd in the editor.
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	if (!PressurePlate)
	{
		return TotalMass;
	}

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

