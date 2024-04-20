// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent *physicsHandle = GetPhysicsHandle();
	if (physicsHandle == nullptr)
	{
		return;
	}

	if (physicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector targetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		physicsHandle->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Releasa graber from blueprint"));
	UPhysicsHandleComponent *physicsHandle = GetPhysicsHandle();
	if (physicsHandle == nullptr)
	{
		return;
	}
	if (physicsHandle->GetGrabbedComponent() != nullptr)
	{
		AActor *actor = physicsHandle->GetGrabbedComponent()->GetOwner();
		actor->Tags.Remove("Grabbed");
		physicsHandle->ReleaseComponent();
	}
}

void UGrabber::Grab()
{

	UPhysicsHandleComponent *physicsHandle = GetPhysicsHandle();
	if (physicsHandle == nullptr)
	{
		return;
	}

	FHitResult result;

	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector() * MaxGrabDistance;

	DrawDebugSphere(GetWorld(), end, 10, 10, FColor::Blue, false, 5);

	FCollisionShape sphere = FCollisionShape::MakeSphere(GrabRadius);

	bool HasHit = GetWorld()->SweepSingleByChannel(result, start, end, FQuat::Identity, ECC_GameTraceChannel2, sphere);

	if (HasHit)
	{
		UPrimitiveComponent *hitComponent = result.GetComponent();
		hitComponent->WakeAllRigidBodies();
		result.GetActor()->Tags.Add("Grabbed");
		physicsHandle->GrabComponentAtLocationWithRotation(
			hitComponent,
			NAME_None,
			result.ImpactPoint,
			GetComponentRotation());
	}
}

UPhysicsHandleComponent *UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent *physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	return physicsHandle;
}
