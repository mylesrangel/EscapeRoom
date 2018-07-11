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

		///UE_LOG(LogTemp, Warning, TEXT("%s has Input Componentt"), *GetOwner()->GetName())

		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {


	}
}

void UGrabber::FindPhysicsHandleComponent(){
	///look for physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle) {
		///found the handle
	}
	else {
		///log error of handle not found
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())

	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabber key pressed"))

		///try and reach a component with a physics body collision channel set
		auto HitResult = GetFirstPhysicsBodyInReach();
		auto ComponentToGrab = HitResult.GetComponent();
		auto ActorHit = HitResult.GetActor();
	
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
	UE_LOG(LogTemp, Warning, TEXT("Grabber key Released"))

		///Release Physics Handle
		PhysicsHandle->ReleaseComponent();

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator);

	//UE_LOG(LogTemp, Warning, TEXT("location: %s Rotator: %s"),
	//	*PlayerViewPointLocation.ToString(),
	//	*PlayerViewPointRotator.ToString())

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;


	///If the Physics Handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		///Move the object that we have
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// ...
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator);

	//UE_LOG(LogTemp, Warning, TEXT("location: %s Rotator: %s"),
	//	*PlayerViewPointLocation.ToString(),
	//	*PlayerViewPointRotator.ToString())

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;

	////Debug line (Red Line)
	//DrawDebugLine(GetWorld(),
	//			PlayerViewPointLocation,
	//			LineTraceEnd, 
	//			FColor(255,0,0),
	//			false,
	//			0.0f,
	//			0.0f,
	//			10.0f
	//);

	///Set up Query Parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());


	//Line-Trace(AKA Ray Casting) to reach distance (Line trace determines what debug line is looking at)

	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters

	);
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(ActorHit->GetName()))
	}

	return Hit;
}

