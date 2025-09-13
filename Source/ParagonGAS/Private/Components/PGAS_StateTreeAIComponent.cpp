/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * This is a sample project demonstrating the use of Unreal Engine's Gameplay Ability System (GAS) & open world.
 * Author: Christopher D. Parker
 * Date: 7-22-2025
 * =============================================================================
 * Implementation of custom StateTree Component for enhanced debugging and control.
 */

#include <Components/PGAS_StateTreeAIComponent.h>
#include "StateTreeSchema.h"


// Constructor
UPGAS_StateTreeAIComponent::UPGAS_StateTreeAIComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    SetStartLogicAutomatically(true);  // Do not start logic automatically on BeginPlay
}

void UPGAS_StateTreeAIComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPGAS_StateTreeAIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/** Called when the component is initialized */
void UPGAS_StateTreeAIComponent::InitializeComponent()
{
    Super::InitializeComponent();
}

/** Validates the State Tree reference */
void UPGAS_StateTreeAIComponent::ValidateStateTreeReference()
{
    Super::ValidateStateTreeReference();
}

/**
 * Sets the State Tree reference for this component and starts the logic.
 * @param StateTreeReference The State Tree reference to set.
*/
void UPGAS_StateTreeAIComponent::StartStateTree(UStateTree* StateTree)
{
    if (StateTree)
    {
        if (IsRunning())
        {
            Cleanup();
        }

        StateTreeRef.SetStateTree(StateTree);
        Super::SetStateTree(StateTree);
        StartLogic();
    }
}

/**
 * Stops the currently running State Tree logic.
 */
void UPGAS_StateTreeAIComponent::StopStateTree()
{
    if (IsRunning())
    {
        Cleanup();
    }
    
    StopLogic(FString("Stopped by Code"));
    Super::SetStateTree(nullptr);
    StateTreeRef.SetStateTree(nullptr);
}

/**
 * Sends an event to the State Tree.
 * @param EventTag The gameplay tag representing the event to send.
*/
void UPGAS_StateTreeAIComponent::SendEvent(const FGameplayTag& EventTag)
{
    if (IsRunning() && LastEventTag != EventTag)
    {
        LastEventTag = EventTag; // Update the last event tag to prevent duplicates
        FStateTreeEvent Event = FStateTreeEvent(EventTag);
        SendStateTreeEvent(Event); // Send the event to the StateTree execution context
        // UE_LOG(LogTemp, Warning, TEXT("UPGAS_StateTreeAIComponent::SendEvent - Event '%s' sent."), *EventTag.GetTagName().ToString());
    }
    else
    {
        // UE_LOG(LogTemp, Warning, TEXT("UPGAS_StateTreeAIComponent::SendEvent - StateTree execution context is not valid. Event '%s' not sent."), *EventTag.GetTagName().ToString());
    }
}
