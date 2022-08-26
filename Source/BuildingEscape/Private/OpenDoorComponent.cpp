// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoorComponent.h"

// Sets default values for this component's properties
UOpenDoorComponent::UOpenDoorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UOpenDoorComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//poll the trigger
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if it is time to close the door;
	//现在得时间 > 开门＋延迟
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}

}

void UOpenDoorComponent::OpenDoor() const
{
	//pitch 点头 //yaw 摇头 	//roll 偏头
	Owner->SetActorRotation(FRotator(0.0f,OpenAngle,0.0f));
}

void UOpenDoorComponent::CloseDoor() const
{
	//pitch 点头 //yaw 摇头 	//roll 偏头
	Owner->SetActorRotation(FRotator(0.0f,0.0f,0.0f));
}
