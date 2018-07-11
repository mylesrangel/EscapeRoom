// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h" //header for GetWorld()



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

	// ...

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	Owner = GetOwner();
	
	
}

void UOpenDoor::OpenDoor() //opens door 60 degrees or so
{

	Owner->SetActorRotation(FRotator(0.0F, OpenAngle, 0.0F));


}

void UOpenDoor::CloseDoor() //opens door 60 degrees or so
{
	Owner->SetActorRotation(FRotator(0.0F, 0.0F, 0.0F));

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) { //if actor is in the pressure plate
		OpenDoor();
		LastDoorOpen = GetWorld()->GetTimeSeconds(); //keep track of how long door is open
		//UE_LOG(LogTemp, Warning, TEXT("LastDoorOpen value %s", LastDoorOpen));
	}
	if ((GetWorld()->GetTimeSeconds() - LastDoorOpen) > CloseDoorDelay) {
		CloseDoor();
	}


	


}

