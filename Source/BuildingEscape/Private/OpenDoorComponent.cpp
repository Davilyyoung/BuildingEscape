// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoorComponent.h"
#define OUT

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
	if (!PressurePlate)
	{
		UE_LOG(LogTemp,Error,TEXT("%s missing PressurePlate"),*GetOwner()->GetName());
	}
	
}

// Called every frame
void UOpenDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	//　poll the trigger volume
	//poll the trigger
	if (GetTotalMassOfActorOnPlate() > 50.0f) //TODO make into aparameter
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
	//Owner->SetActorRotation(FRotator(0.0f,OpenAngle,0.0f));
	OnOpenRequest.Broadcast();
}

void UOpenDoorComponent::CloseDoor() const
{
	//pitch 点头 //yaw 摇头 	//roll 偏头
	//Owner->SetActorRotation(FRotator(0.0f,0.0f,0.0f));
	OnCloseRequest.Broadcast();
}

float UOpenDoorComponent::GetTotalMassOfActorOnPlate()
{
	float TotalMass = 0.0f;
	//find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate){return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	//iterate through them adding their masses
	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp,Warning,TEXT("%s on pressure plate"), *Actor->GetName());
	}
	
	return TotalMass;
}
