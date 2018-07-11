// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h" //header for GetWorld()

#define OUT



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
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor() //opens door 60 degrees or so
{
	Owner->SetActorRotation(FRotator(0.0F, OpenAngle, 0.0F));
	LastDoorOpen = GetWorld()->GetTimeSeconds();
}

void UOpenDoor::CloseDoor() //opens door 60 degrees or so
{
	Owner->SetActorRotation(FRotator(0.0F, 0.0F, 0.0F));

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > 20.0f) //TODO change to variable
	{
		OpenDoor();
		LastDoorOpen = GetWorld()->GetTimeSeconds(); //keep track of how long door is open
		//UE_LOG(LogTemp, Warning, TEXT("LastDoorOpen value %s", LastDoorOpen));
	}
	if ((GetWorld()->GetTimeSeconds() - LastDoorOpen) > CloseDoorDelay) {
		CloseDoor();
	}


	


}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	//Find Overlapping actors
	TArray<AActor*> OverLappingActors;
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);

	//iterate through them adding their mass

	for (const auto* Actor : OverLappingActors) {
		
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on pressure plate"), *Actor->GetName())
	}
	return TotalMass;
}

