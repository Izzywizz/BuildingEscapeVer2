

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Public/DrawDebugHelpers.h"

#define OUT //a macro that does absolutely nothing, it substitues nothing in, 
//we do this to remind ourseleves that we are modifying the variables we are passing in (directly, they are not copies)

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Returns two vectors, one is the Starting point (v1) of the tracing line from the character and the 2nd vector is the ending point(v2) ie its reach
FTwoVectors UGrabber::GetLineTracePoints() const
{

	// Get the player's viewpoint this frame/tick: where they are, where are they looking
	FVector StartLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT StartLocation,
		OUT PlayerViewPointRotation
	);
	FVector EndLocation = StartLocation + PlayerViewPointRotation.Vector() * Reach;
	return FTwoVectors(StartLocation, EndLocation);
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}

// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	if (!PhysicsHandle) { return; } //prevents nullptr exceptions
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

// Look for attached Input componet
void UGrabber::SetupInputComponent()
{
	if (!InputComponent) { return; } //prevents nullptr exceptions

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//Found Input component
		UE_LOG(LogTemp, Warning, TEXT("Input component found"));
		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		//So we are providing the address of the grab method which is located in THIS component/obj hence the this 
		//and when either of the keys that are mapped to the Grab(string) action from within the editor/Input settings which are PRESSED (IE_Pressed)

		//release key press
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component Not found within %s "), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; } //prevents nullptr exceptions
	if (!InputComponent) { return; } //prevents nullptr exceptions

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object we're holding
		PhysicsHandle->SetTargetLocation(GetLineTracePoints().v2); // we set the targets location to the end of the reach line, v2 is the end point vector
	}

}


// Creates A line Trace/ Ray cast Hit object that indicates what Physics object is infront of the user, ie Chair or table 
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//draw a red tracing line to visual where we are pointing
//This takes the intial location of where the player is (relative to the origin [0,0,0]) + 
//Direction of where the player has rotated thier head * (multiplied) by the reach of where we want to grab 
//(we multiply it by the reach because the roatated vector is normailsed to one)
//Drawing red line (explanation above)
/*DrawDebugLine(GetWorld(),
	GetLineTracePoints().v1,
	GetLineTracePoints().v2,
	FColor(255, 0, 0),
	false,
	0.0f,
	0.0f,
	10.0f
);*/

	/// Line-trace(Ray casting)  laser out to reach distance
	//The idea for picking up objects is we have is to filter by Object Channel filtering for collisions, 
	// so we can only interact with Physics bodies which is its Object Type (objs we set as having physic simulated and have mass will tag it as Physics Bodies)

	/// Setup querry parameters
	//we ignore ourselevs and we don't do a complex collision visbiility, ignore tag for now
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); 

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,  //this method bc of out parameter changes the parameters directly within the method
		GetLineTracePoints().v1, //starting point
		GetLineTracePoints().v2, //ending reach point
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		FString Name = ActorHit->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit:: %s"), *Name);
	}
	/// See what we hit

	return HitResult;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Keys have been pressed!"));
	/// LINE TRACE  and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); //Why a component not an actor? explanation here: https://community.gamedev.tv/t/warning-dumb-question-why-are-we-hitting-the-component/76351/2
	auto ActorHit = HitResult.GetActor();

	if (!PhysicsHandle) { return; } //prevents nullptr exceptions

	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		// Using the commented out one doesn't take in to account the rotation of the object you're holding
		//PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation());
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, // The component we're grabbing is the mesh (see explanation above)
			NAME_None, // No Bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(), // grab the component (physics mesh)
			FRotator(0.0f, 0.0f, 0.0f)); //prevents the object from rotating, fixed
	}

}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; } //prevents nullptr exceptions

	UE_LOG(LogTemp, Warning, TEXT("Grab Keys Released! Drop object"));
	PhysicsHandle->ReleaseComponent();
	// TODO release physics handle
}

