/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * This file implements the evaluator that binds to AIController's perception delegates.
 * Author: Christopher D. Parker
 * Date: 7-28-2025
 * =============================================================================
 * This file defines the base StateTree Evaluator that listens to AIController's perception events.
 */

#include "StateTrees/Evaluators/PGAS_STE_PerceptionBase.h"
#include "Controllers/AI/PGAS_EnemyAIController.h"
#include "StateTreeExecutionContext.h"

void UPGAS_STE_PerceptionBase::TreeStart(FStateTreeExecutionContext& Context)
{
    Super::TreeStart(Context);

    // Set our context character
    Character = Cast<APGAS_EnemyCharacter>(Context.GetOwner()); // This is the actor that this evaluator will operate on

    // Set our context ai controller
    APawn* OwnerPawn = Cast<APawn>(Context.GetOwner());
    if (!OwnerPawn) return;
    
    APGAS_EnemyAIController* aiController = Cast<APGAS_EnemyAIController>(OwnerPawn->GetController());
    if (!aiController) return;

    AIController = aiController;

    // Bind to controller's delegates (avoid duplicate binds)
    AIController->OnSightStimulusDetected.AddDynamic(this, &UPGAS_STE_PerceptionBase::HandleSightStimulus);
    AIController->OnSightStimulusForgotten.AddDynamic(this, &UPGAS_STE_PerceptionBase::HandleSightStimulusForgotten);
    AIController->OnHearingStimulusDetected.AddDynamic(this, &UPGAS_STE_PerceptionBase::HandleHearingStimulus);
    AIController->OnHearingStimulusForgotten.AddDynamic(this, &UPGAS_STE_PerceptionBase::HandleHearingStimulusForgotten);
    AIController->OnDamageStimulusDetected.AddDynamic(this, &UPGAS_STE_PerceptionBase::HandleDamageStimulus);
}

void UPGAS_STE_PerceptionBase::TreeStop(FStateTreeExecutionContext& Context)
{
    Super::TreeStop(Context);

    if (AIController.IsValid())
    {
        AIController->OnSightStimulusDetected.RemoveDynamic(this, &UPGAS_STE_PerceptionBase::HandleSightStimulus);
        AIController->OnSightStimulusForgotten.RemoveDynamic(this, &UPGAS_STE_PerceptionBase::HandleSightStimulusForgotten);
        AIController->OnHearingStimulusDetected.RemoveDynamic(this, &UPGAS_STE_PerceptionBase::HandleHearingStimulus);
        AIController->OnHearingStimulusForgotten.RemoveDynamic(this, &UPGAS_STE_PerceptionBase::HandleHearingStimulusForgotten);
        AIController->OnDamageStimulusDetected.RemoveDynamic(this, &UPGAS_STE_PerceptionBase::HandleDamageStimulus);
    }

    // Clear the AIController reference
    AIController = nullptr;
}

// Broadcast handlers for perception events
void UPGAS_STE_PerceptionBase::HandleDamageStimulus(AActor* Actor, const FAIStimulus& Stimulus)
{
    OnDamageStimulus(Actor, Stimulus);
}

void UPGAS_STE_PerceptionBase::HandleSightStimulus(AActor* Actor, const FAIStimulus& Stimulus)
{
    OnSightStimulus(Actor, Stimulus);
}

void UPGAS_STE_PerceptionBase::HandleSightStimulusForgotten(AActor* Actor)
{
    OnSightStimulusForgotten(Actor);
}

void UPGAS_STE_PerceptionBase::HandleHearingStimulus(AActor* Actor, const FAIStimulus& Stimulus)
{
    OnHearingStimulus(Actor, Stimulus);
}

void UPGAS_STE_PerceptionBase::HandleHearingStimulusForgotten(AActor* Actor)
{
    OnHearingStimulusForgotten(Actor);
}
