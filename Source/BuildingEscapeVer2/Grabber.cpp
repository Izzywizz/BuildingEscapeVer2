

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
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		// Physics handle is found
		UE_LOG(LogTemp, Warning, TEXT("Physics Handler Found!"));
	}
	else
	{
		FString name = GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("Physics Handler Not found within %s "), *name);
	}
}

// Look for attached Input componet
void UGrabber::SetupInputComponent()
{
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

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object we're holding
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotator;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation, //this method bc of out parameter changes the parameters directly within the method
			OUT PlayerViewPointRotator
		);

		FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotator.Vector() * Reach);
		PhysicsHandle->SetTargetLocation(LineTraceEnd); // we set the targets location to the end of the reach line
	}

}

// Creates A line Trace/ Ray cast Hit object that indicates what Physics object is infront of the user, ie Chair or table 
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get the player's viewpoint this frame/tick: where they are, where are they looking
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, //this method bc of out parameter changes the parameters directly within the method
		OUT PlayerViewPointRotator
	);
	//log out to test
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s and Rotation: %s"), 
	//	*PlayerViewPoint.ToString(), 
	//	*PlayerViewPointRotator.ToString());

	//draw a red tracing line to visual where we are pointing
	//This takes the intial location of where the player is (relative to the origin [0,0,0]) + 
	//Direction of where the player has rotated thier head * (multiplied) by the reach of where we want to grab 
	//(we multiply it by the reach because the roatated vector is normailsed to one)

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotator.Vector() * Reach);
	//Drawing red line (explanation above)
	/*DrawDebugLine(GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
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
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); //we ignore ourselevs and we don't do a complex collision visbiility, ignore tag for now
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		FString Name = ActorHit->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit:: %s"), *Name);
	}
	/// See what we hit

	return Hit;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Keys have been pressed!"));

	/// LINE TRACE  and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		// TODO attach physics handle
		//PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation());
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
	}

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Keys Released! Drop object"));
	PhysicsHandle->ReleaseComponent();
	// TODO release physics handle
}

