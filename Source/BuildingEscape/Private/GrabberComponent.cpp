#include "GrabberComponent.h"
#include "DrawDebugHelpers.h"

#define OUT

UGrabberComponent::UGrabberComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}

void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandleComp){return;}
	// if the physics handle is attach
	if (PhysicsHandleComp->GrabbedComponent)
	{
		// move the object that we are holding
		PhysicsHandleComp->SetTargetLocation(GetReachLineEnd());
	}
}

// look for attached physics handle Compnent
void UGrabberComponent::FindPhysicsHandleComponent()
{
	PhysicsHandleComp = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandleComp == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("%s missing PhysicsHandleComponent"),*GetOwner()->GetName());
	}
}

// look for attached Input Compnent(only appears at run time)
void UGrabberComponent::SetupInputComponent()
{
	InputComp = GetOwner()->FindComponentByClass<UInputComponent>();
	
	if (InputComp)
	{
		// bind the input asix
		InputComp->BindAction("Grab",EInputEvent::IE_Pressed,this,&UGrabberComponent::Grab);
		InputComp->BindAction("Grab",EInputEvent::IE_Released,this,&UGrabberComponent::Release);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("%s missing InputComponent"),*GetOwner()->GetName());
	}
}

FHitResult UGrabberComponent::GetFirstPhysicsBodyInReach() const
{
	/// Setup query parmeters
	FCollisionQueryParams TraceParmeters(FName(TEXT("")),false,GetOwner());
	FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);
	/// Line-trace(AKA ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		ObjectQueryParams,
		TraceParmeters);
	
	return HitResult;
}

FVector UGrabberComponent::GetReachLineEnd() const
{
	// Get player view point this tick
	FVector PlayerViewPointlocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointlocation,
		OUT PlayerViewPointRotation);
	
	//Draw a red trace int the world to visual
	//direction = PlayerViewPointRotation.Vector()
	return PlayerViewPointlocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabberComponent::GetReachLineStart() const
{
	// Get player view point this tick
	FVector PlayerViewPointlocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointlocation,
		OUT PlayerViewPointRotation);
	
	//Draw a red trace int the world to visual
	//direction = PlayerViewPointRotation.Vector()
	return PlayerViewPointlocation;
}

void UGrabberComponent::Grab()
{
	// LINE TRACE and see if we  reach any actors with physics body collosion chanel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	// if we hit something then attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandleComp){return;}
		PhysicsHandleComp->GrabComponentAtLocationWithRotation(
			ComponentToGrab,//
			NAME_None,//no bone 
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation());
	}

}

void UGrabberComponent::Release()
{
	//Release physics handle
	if (!PhysicsHandleComp){return;}
	PhysicsHandleComp->ReleaseComponent();
}


