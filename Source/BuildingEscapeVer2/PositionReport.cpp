// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReport.h"
#include "GameFramework/Actor.h" //getOwner autocomplete

// Sets default values for this component's properties/ CONSTRUCTOR!!
UPositionReport::UPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts like start in unity
void UPositionReport::BeginPlay()
{
	Super::BeginPlay();
	// ...
	FString ObjectName = GetOwner()->GetName();
	FString ActorLocation = GetOwner()->GetActorLocation().ToString();
	FString ObjectPos = ("Position: %s", ActorLocation);

	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *ObjectName, *ObjectPos);
 }


// Called every frame bit like update on unity
void UPositionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
