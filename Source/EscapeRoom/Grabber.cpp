// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "Math/Vector.h"

#define OUT //just a reminder that the content changes

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...s
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();

	
}

//look for attached Input Component (Only appears at runtime)
void UGrabber::SetupInputComponent(){

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {

		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		///TODO Log a warning if missing input component

	}
}

void UGrabber::FindPhysicsHandleComponent(){
	///look for physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr) {
		///log error of handle not found
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab() {
		///try and reach a component with a physics body collision channel set
		auto HitResult = GetFirstPhysicsBodyInReach();
		auto ComponentToGrab = HitResult.GetComponent(); //Gets mesh in our case
		auto ActorHit = HitResult.GetActor();
	
		if (!PhysicsHandle) { return; }
		///if we hit something attach a Physics handle
		if (ActorHit) {
			///Atach Physics Handle
			PhysicsHandle->GrabComponent(
				ComponentToGrab,
				NAME_None,
				ComponentToGrab->GetOwner()->GetActorLocation(),
				true //allow rotation
			);
		}
}

void UGrabber::Release() {

	///Release Physics Handle
	if (!PhysicsHandle) { return; } ///if there is no physics handle bail out of function
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	///If the Physics Handle is attached
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent) {
		///Move the object that we have
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Line-Trace(AKA Ray Casting) to reach distance (Line trace determines what debug line is looking at)
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters

	);
	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator);

	//UE_LOG(LogTemp, Warning, TEXT("location: %s Rotator: %s"),
	//	*PlayerViewPointLocation.ToString(),
	//	*PlayerViewPointRotator.ToString())

	return PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;
}
