// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h" //should be last header always


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	void OpenDoor();
	void CloseDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float CloseDoorDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	float OpenAngle = -90.f;

	float LastDoorOpen;

	AActor* Owner = nullptr; //find the Owner of the item (door in this case)

	//Returns total mass in KG
	float GetTotalMassOfActorsOnPlate();

	


	
};
