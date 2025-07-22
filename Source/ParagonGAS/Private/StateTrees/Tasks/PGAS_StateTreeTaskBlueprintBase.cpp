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
 * Date: 7-23-2025
 * =============================================================================
 * UPGAS_StateTreeTaskBlueprintBase
 * Implementation file for blueprint-friendly StateTree task base class.
 */


#include "StateTrees/Tasks/PGAS_StateTreeTaskBlueprintBase.h"


 // Called when the state is entered.
void UPGAS_StateTreeTaskBlueprintBase::OnEnterState_Implementation(const FStateTreeTransitionResult& Transition)
{
    // You can add base logging or logic here if needed
    // UE_LOG(LogTemp, Log, TEXT("[%s] Entered State"), *GetName());
}

// Called when the state is exited.
void UPGAS_StateTreeTaskBlueprintBase::OnExitState_Implementation(const FStateTreeTransitionResult& Transition)
{
    // Base logic for all derived tasks, if needed
    // UE_LOG(LogTemp, Log, TEXT("[%s] Exited State"), *GetName());
}

// Called every tick if enabled.
void UPGAS_StateTreeTaskBlueprintBase::OnTick_Implementation(float DeltaTime)
{
    // Base logic for tick, if needed
    // UE_LOG(LogTemp, Verbose, TEXT("[%s] Ticking State: DeltaTime=%f"), *GetName(), DeltaTime);
}

// Called when the state is completed.
void UPGAS_StateTreeTaskBlueprintBase::OnStateCompleted_Implementation(EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates)
{
    // Base logic for completion, if needed
    // UE_LOG(LogTemp, Log, TEXT("[%s] State Completed: Status=%d"), *GetName(), (int32)CompletionStatus);
}
