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
 * Date: 7-31-2025
 * =============================================================================
 * PGAS_HealthEvaluator.h
 * Evaluator that monitors the Context Actor's health attribute and exposes it to the State Tree as CurrentHealth.
 */

#include <StateTrees/Evaluators/PGAS_STE_HealthEvaluator.h>
#include "StateTreeExecutionContext.h"

void UPGAS_STE_HealthEvaluator::TreeStart(FStateTreeExecutionContext& Context)
{
    Super::TreeStart(Context);

    CurrentHealth = 0.f;
}

void UPGAS_STE_HealthEvaluator::Tick(FStateTreeExecutionContext& Context, float DeltaTime)
{
    Super::Tick(Context, DeltaTime);

    if (Character)
    {
        CurrentHealth = Character->GetHealth(); // Get the current health from the character's attribute set
    }
    else
    {
        CurrentHealth = 0.f;
    }
}

void UPGAS_STE_HealthEvaluator::TreeStop(FStateTreeExecutionContext& Context)
{
    Super::TreeStop(Context);    
}