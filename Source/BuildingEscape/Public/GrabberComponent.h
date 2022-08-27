#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabberComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabberComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	
	float Reach = 100.0f;

	UPROPERTY(VisibleDefaultsOnly)
	UPhysicsHandleComponent* PhysicsHandleComp = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UInputComponent* InputComp = nullptr;

	//Ray-cast and Grab what is in reach
	void Grab();
	void Release();
	
	//find attached PhysicsHandleComp
	void FindPhysicsHandleComponent();
	//setup (assumed) attached InputComp
	void SetupInputComponent();
	//Return hit for first physics body in reach
	FHitResult GetFirstPhysicsBodyInReach() const;

	//Return current end of reach line
	FVector GetReachLineEnd() const;
	FVector GetReachLineStart() const;
	
};
