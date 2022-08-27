// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void OpenDoor() const;
	void CloseDoor() const;

	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpenRequest;
	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnCloseRequest;
	
	
private:
	UPROPERTY(EditDefaultsOnly)
	float OpenAngle = 90.0f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	float DoorCloseDelay = 1.0f;

	float LastDoorOpenTime;
	
	AActor* Owner = nullptr;// the owning door

	//Returns total mass in kg
	float GetTotalMassOfActorOnPlate();
	
};
