// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Display, TEXT("Hello there!! Is trigger component"));
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor *actor = GetAcceptableActor();

    if (actor != nullptr)
    {
        if (actor->Tags.Find(TEXT("Grabbed")) == INDEX_NONE)
        {

            if (mover != nullptr)
            {
                mover->SetShouldMove(true);
            }
            UPrimitiveComponent *component = Cast<UPrimitiveComponent>(actor->GetRootComponent());
            if (component != nullptr)
            {
                component->SetSimulatePhysics(false);
            }
            actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
    else
    {
        // mover->SetShouldMove(false);
    }
}

void UTriggerComponent::SetMover(UMover *newMover)
{
    mover = newMover;
}

AActor *UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor *> Actors;
    GetOverlappingActors(Actors);

    for (AActor *Actor : Actors)
    {
        if (Actor->ActorHasTag(TagSpected))
        {
            return Actor;
        }
    }
    return nullptr;
}
