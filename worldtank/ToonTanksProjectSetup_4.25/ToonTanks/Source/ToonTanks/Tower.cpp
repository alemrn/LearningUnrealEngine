// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::CheckFireCondition()
{
    if (tank == nullptr)
    {
        return;
    }

    if (tank && tank->bAlive)
    {
        float distance = FVector::Dist(GetActorLocation(), tank->GetActorLocation());
        /// Check to see if the Tank is in range
        if (distance <= minDistance)
        {
            Fire();
        }
    }
}

void ATower::BeginPlay()
{
    Super::BeginPlay();

    tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &ATower::CheckFireCondition, fireRate, true);
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (tank)
    {
        /// Find the distance to the Tank
        float distance = FVector::Dist(GetActorLocation(), tank->GetActorLocation());
        /// Check to see if the Tank is in range
        if (distance <= minDistance)
        {
            /// If in range, rotate turrent toward tank
            RotateTurret(tank->GetActorLocation(), 20.f);
        }
    }
}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}
