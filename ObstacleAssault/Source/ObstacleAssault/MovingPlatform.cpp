// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	FString Name = GetName();

	UE_LOG(LogTemp, Display, TEXT("Here's My String : %s"), *Name);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	FString Name = GetName();

	// Move platform forwards
	// Get current location
	FVector CurrentLocation = GetActorLocation();
	// Add vector to that location
	CurrentLocation = CurrentLocation + PlatformVelocity * DeltaTime;
	// Set the locat ion
	SetActorLocation(CurrentLocation);

	// Send platform back if gone too far
	// Check how far we've moved
	distance = GetDistanceMoved(CurrentLocation);
	// Reverse direction of motion if gone too far
	if (shouldPlatformReturn())
	{
		overShoot = distance - distanceMove;
		UE_LOG(LogTemp, Display, TEXT("%s Overshoot is %f"), *Name, overShoot);
		FVector MoveDirection = PlatformVelocity.GetSafeNormal();
		StartLocation = StartLocation + MoveDirection * distanceMove;
		SetActorLocation(StartLocation);
		PlatformVelocity = -PlatformVelocity;
	}
}

double AMovingPlatform::GetDistanceMoved(FVector &CurrentLocation)
{
	return FVector::Dist(CurrentLocation, StartLocation);
}

bool AMovingPlatform::shouldPlatformReturn() const
{
	return distance >= distanceMove;
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	AddActorLocalRotation(rotationVelocity * DeltaTime);
}
