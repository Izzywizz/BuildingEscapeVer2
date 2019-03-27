

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h" //UPhysicsHandleComponent
#include "Components/InputComponent.h" //UInputComponent
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPEVER2_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//how far ahead of the player we can reach in cm
	float Reach = 100.0f;

	UPhysicsHandleComponent* PhyicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;
	
	// Ray-cast and grab whats in reach
	void Grab();
	// Called when the Grab have been released
	void Release();
};
