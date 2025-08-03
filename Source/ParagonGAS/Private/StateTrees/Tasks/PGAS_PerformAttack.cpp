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
 * Date: 7-30-2025
 * =============================================================================
 * PGAS_PerformAttack
 * A StateTree task that selects and triggers a randomized attack ability via GAS.
 */

#include "StateTrees/Tasks/PGAS_PerformAttack.h"
#include "AbilitySystemInterface.h"
#include "Math/UnrealMathUtility.h"


EStateTreeRunStatus UPGAS_PerformAttackTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    // Use the context-provided Actor directly
    if (!Actor)
    {
        UE_LOG(LogTemp, Warning, TEXT("PerformAttackTask: Actor context is null."));
        return EStateTreeRunStatus::Failed;
    }

    // Check for tags
    if (AttackAbilityTags.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("PerformAttackTask: No attack ability tags assigned."));
        return EStateTreeRunStatus::Failed;
    }

    // Choose index
    int32 ChosenIndex = 0;
    if (SelectionMode == EPGAS_AttackSelectMode::Random)
    {
        ChosenIndex = FMath::RandRange(0, AttackAbilityTags.Num() - 1);
    }
    else // Sequential
    {
        static int32 SequentialIdx = 0;
        ChosenIndex = SequentialIdx++ % AttackAbilityTags.Num();
    }

    // Activate via GAS
    FGameplayTagContainer Tags;
    Tags.AddTag(AttackAbilityTags[ChosenIndex]);
    if (!Actor->ActivateAbilitiesWithTags(Tags))
    {
        UE_LOG(LogTemp, Warning, TEXT("PerformAttackTask: Failed to activate ability for tag %s"), *Tags.ToStringSimple());
        return EStateTreeRunStatus::Failed;
    }

    Actor->SetIsAttacking(true);
    return EStateTreeRunStatus::Succeeded;
}