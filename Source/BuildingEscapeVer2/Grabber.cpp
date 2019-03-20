

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"

#define OUT //a macro that does absolutely nothing, it substitues nothing in, 
//we do this to remind ourseleves that we are modifying the variables we are passing in (directly, they are not copies)

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

	// ...
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	DrawDebugLine(GetWorld(), 
		PlayerViewPointLocation, 
		LineTraceEnd, 
		FColor(255, 0, 0), 
		false, 
		0.0f, 
		0.0f, 
		10.0f
	);
	// Ray casting  laser out to reach distance

	// See what we hit
}
