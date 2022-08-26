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

	// Get player view point this tick
	FVector PlayerViewPointlocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointlocation,
		OUT PlayerViewPointRotation);
	
	//Draw a red trace int the world to visual
	//direction = PlayerViewPointRotation.Vector()
	FVector LineTraceEnd = PlayerViewPointlocation + PlayerViewPointRotation.Vector() * Reach;
	
	// if the physics handle is attach
	if (PhysicsHandleComp->GrabbedComponent)
	{
		// move the object that we are holding
		PhysicsHandleComp->SetTargetLocation(LineTraceEnd);
	}
		
	
}

// look for attached physics handle Compnent
void UGrabberComponent::FindPhysicsHandleComponent()
{

	PhysicsHandleComp = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandleComp)
	{
		// PhysicsHandleComp is find
	}
	else
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
		// InputCompnent is find
		UE_LOG(LogTemp,Warning,TEXT("InputCompnent is find"));
		// bind the input asix
		InputComp->BindAction("Grab",EInputEvent::IE_Pressed,this,&UGrabberComponent::Grab);
		InputComp->BindAction("Grab",EInputEvent::IE_Released,this,&UGrabberComponent::Release);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("%s missing InputComponent"),*GetOwner()->GetName());
	}
}


const FHitResult UGrabberComponent::GetFirstPhysicsBodyInReach()
{
	// Get player view point this tick
	FVector PlayerViewPointlocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointlocation,
		OUT PlayerViewPointRotation);
	
	//Draw a red trace int the world to visual
	//direction = PlayerViewPointRotation.Vector()
	FVector LineTraceEnd = PlayerViewPointlocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointlocation,
		LineTraceEnd,
		FColor::Red,
		false,
		0.0f,
		0.0f,
		10.0f);
	/// Setup query parmeters
	FCollisionQueryParams TraceParmeters(FName(TEXT("")),false,GetOwner());
	FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);
	/// Line-trace(AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit,
		PlayerViewPointlocation,
		LineTraceEnd,
		ObjectQueryParams,
		TraceParmeters);
	// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp,Warning,TEXT("Line trace hit: %s"),*ActorHit->GetName());
	}

	return Hit;
}




void UGrabberComponent::Grab()
{
	//Grab item
	UE_LOG(LogTemp,Warning,TEXT("Grab pressed"));

	// LINE TRACE and see if we  reach any actors with physics body collosion chanel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	// if we hit something then attach a physics handle
	if (ActorHit)
	{
		// attach physics handle
		PhysicsHandleComp->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation());
	}

}

void UGrabberComponent::Release()
{
	UE_LOG(LogTemp,Warning,TEXT("Grab Release"));

	//Release physics handle
	PhysicsHandleComp->ReleaseComponent();
	
	
}


