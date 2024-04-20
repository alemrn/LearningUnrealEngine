// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATank::ATank()
{
    springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Sprint Arm component"));
    springArmComponent->SetupAttachment(RootComponent);
    cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera component"));
    cameraComponent->SetupAttachment(springArmComponent);
}

void ATank::Move(float value)
{
    FVector DeltaLocation = FVector::ZeroVector;
    float DeltaTime = UGameplayStatics ::GetWorldDeltaSeconds(this);

    DeltaLocation.X = value * DeltaTime * speed;
    AddActorLocalOffset(DeltaLocation);
}

void ATank::Turn(float value)
{

    float DeltaTime = UGameplayStatics ::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(FRotator(0, value * DeltaTime * turnRate, 0));
}

void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TankPlayerController)
    {
        FHitResult result;
        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, result);

        RotateTurret(result.ImpactPoint, 5.f);
    }
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);

    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;
}

void ATank::BeginPlay()
{
    Super::BeginPlay();
    // playerControllerRef = GetController<APlayerController>();
    TankPlayerController = Cast<APlayerController>(GetController());
}
