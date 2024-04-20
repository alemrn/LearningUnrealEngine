// Fill out your copyright notice in the Description page of Project Settings.

#include "TooknTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "TimerManager.h"

void ATooknTanksGameMode::ActorDied(AActor *DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();

        if (ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnableState(false);
        }
        GameOver(false);
    }
    else if (ATower *DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        TargetTowers--;

        UE_LOG(LogTemp, Display, TEXT("Number of towers %u"), GetTargetTowerCount());

        if (TargetTowers == 0)
        {
            GameOver(true);
        }
    }
}

void ATooknTanksGameMode::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();
}

void ATooknTanksGameMode::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();
    UE_LOG(LogTemp, Display, TEXT("Number of towers %u"), GetTargetTowerCount());
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnableState(false);
        FTimerHandle PlayerEnableTimerHandle;

        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController,
            &AToonTanksPlayerController::SetPlayerEnableState,
            true);

        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);
    }
}

int32 ATooknTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor *> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}
