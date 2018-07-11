// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UGrabber : public UActorComponent
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

	//How far a player can 'reach' something
	float Reach = 100.0f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;


	//Ray-cast and grab what is in reach
	void Grab();
	
	//Ray-cast and button was released
	void Release();

	//find attached physics handle
	void FindPhysicsHandleComponent();

	//Set up (Assumed) attached input component
	void SetupInputComponent();

	//Return hit for first Physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	//Return character reach start
	FVector GetReachLineStart();

	//Return character reach line
	FVector GetReachLineEnd();
	
};
