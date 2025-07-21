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
 * Date: 7-13-2025
 * =============================================================================
 * PGAS_STT_HasGameplayTag
 * Implementation: StateTree Task that checks if an actor has a specified gameplay tag (generic for any tag)
 */

#include "StateTrees/Tasks/PGAS_STT_HasGameplayTag.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
// #include "Engine/World.h"
#include "StateTreeLinker.h"
#include "StateTreeExecutionContext.h"

/**
 * Enter the state tree task. This is called when the task is first executed.
 * @param Context The execution context for the state tree.
 * @param Transition The transition that triggered this task.
 * @return The status of the task after entering.
*/
EStateTreeRunStatus FPGAS_STT_HasGameplayTagTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
    // if (!InstanceData)
    //     return EStateTreeRunStatus::Failed;

    AActor* TargetActor = Cast<AActor>(Context.GetOwner());
    if (!TargetActor || !InstanceData.TagToCheck.IsValid())
        return EStateTreeRunStatus::Failed;

    UE_LOG(LogTemp, Verbose, TEXT("[PGAS_STT_HasGameplayTag] EnterState. TargetActor: %s, TagToCheck: %s"),
        TargetActor ? *TargetActor->GetName() : TEXT("None"),
        *InstanceData.TagToCheck.ToString());

    // Ready to run
    return EStateTreeRunStatus::Running;
}

/**
 * Tick the task. This is called every frame while the task is active.
 * @param Context The execution context for the state tree.
 * @param DeltaTime The time since the last tick.
 * @return The status of the task after ticking.
*/
EStateTreeRunStatus FPGAS_STT_HasGameplayTagTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
    // if (!InstanceData.IsValid())
    //     return EStateTreeRunStatus::Failed;
    
    AActor* TargetActor = Cast<AActor>(Context.GetOwner());
    if (!TargetActor || !InstanceData.TagToCheck.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("[PGAS_STT_HasGameplayTag] Tick FAILED: TargetActor or TagToCheck not valid!"));
        return EStateTreeRunStatus::Failed;
    }

    bool bFound = false;

    // Prefer the GAS interface if available
    if (const IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(TargetActor))
    {
        if (UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent())
        {
            bFound = ASC->HasMatchingGameplayTag(InstanceData.TagToCheck);
        }
    }
    else if (const IGameplayTagAssetInterface* TagAssetInterface = Cast<IGameplayTagAssetInterface>(TargetActor))
    {
        bFound = TagAssetInterface->HasMatchingGameplayTag(InstanceData.TagToCheck);
    }

    // Write the result for StateTree
    const_cast<FPGAS_STT_HasGameplayTagTask*>(this)->bHasTag = bFound;

    return EStateTreeRunStatus::Succeeded;
}

/**
 * Exit the state tree task. This is called when the task is completed or interrupted.
 * @param Context The execution context for the state tree.
 * @param Transition The transition that triggered the exit.
*/
void FPGAS_STT_HasGameplayTagTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    UE_LOG(LogTemp, Verbose, TEXT("[PGAS_STT_HasGameplayTag] ExitState"));
}

/**
 * Called when the state tree task is completed.
 * @param Context The execution context for the state tree.
 * @param Transition The transition that triggered the completion.
*/
void FPGAS_STT_HasGameplayTagTask::StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates) const
{
    UE_LOG(LogTemp, Verbose, TEXT("[PGAS_STT_HasGameplayTag] StateCompleted"));
}
