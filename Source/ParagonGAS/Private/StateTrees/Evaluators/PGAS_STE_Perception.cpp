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

#include "StateTrees/Evaluators/PGAS_STE_Perception.h"
#include "Controllers/AI/PGAS_EnemyAIController.h"
#include "StateTreeExecutionContext.h"

 /**
  * Initializes the evaluator and binds to the AIController's perception events.
  * This is called when the StateTree starts.
  * @param Context The execution context for the StateTree.
 */
void UPGAS_STE_Perception::TreeStart(FStateTreeExecutionContext& Context)
{
    Super::TreeStart(Context);

    // Set our context ai controller (owner of the StateTree)
    APGAS_EnemyAIController* aiController = Cast<APGAS_EnemyAIController>(Context.GetOwner());
    if (!aiController)
    {
        UE_LOG(LogTemp, Error, TEXT("UPGAS_STE_Perception: StateTree owner is not an APGAS_EnemyAIController!"));
        return;
    }
    AIController = aiController;

    // Bind to controller's delegates (avoid duplicate binds)
    AIController->OnSightStimulusDetected.AddDynamic(this, &UPGAS_STE_Perception::HandleSightStimulus);
    AIController->OnSightStimulusForgotten.AddDynamic(this, &UPGAS_STE_Perception::HandleSightStimulusForgotten);
    AIController->OnHearingStimulusDetected.AddDynamic(this, &UPGAS_STE_Perception::HandleHearingStimulus);
    AIController->OnHearingStimulusForgotten.AddDynamic(this, &UPGAS_STE_Perception::HandleHearingStimulusForgotten);
    AIController->OnDamageStimulusDetected.AddDynamic(this, &UPGAS_STE_Perception::HandleDamageStimulus);

    // Get the Pawn from the AI Controller
    APawn* OwnerPawn = AIController->GetPawn();
    if (!OwnerPawn)
    {
        UE_LOG(LogTemp, Error, TEXT("UPGAS_STE_Perception: AIController has no Pawn!"));
        return;
    }

    // Set our context character
    Character = Cast<APGAS_EnemyCharacter>(OwnerPawn);
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("UPGAS_STE_Perception: Pawn is not an APGAS_EnemyCharacter! Class is: %s"), *OwnerPawn->GetClass()->GetName());
        return;
    }

    // Store StateTreeComp reference
    StateTreeComp = AIController->FindComponentByClass<UStateTreeAIComponent>();
    if (!StateTreeComp)
    {
        UE_LOG(LogTemp, Error, TEXT("UPGAS_STE_Perception: No UStateTreeAIComponent found on AIController!"));
        return;
    }
}

/**
 * Cleans up the evaluator and unbinds from the AIController's perception events.
 * This is called when the StateTree stops.
 * @param Context The execution context for the StateTree.
*/
void UPGAS_STE_Perception::TreeStop(FStateTreeExecutionContext& Context)
{
    Super::TreeStop(Context);

    if (AIController.IsValid())
    {
        AIController->OnSightStimulusDetected.RemoveDynamic(this, &UPGAS_STE_Perception::HandleSightStimulus);
        AIController->OnSightStimulusForgotten.RemoveDynamic(this, &UPGAS_STE_Perception::HandleSightStimulusForgotten);
        AIController->OnHearingStimulusDetected.RemoveDynamic(this, &UPGAS_STE_Perception::HandleHearingStimulus);
        AIController->OnHearingStimulusForgotten.RemoveDynamic(this, &UPGAS_STE_Perception::HandleHearingStimulusForgotten);
        AIController->OnDamageStimulusDetected.RemoveDynamic(this, &UPGAS_STE_Perception::HandleDamageStimulus);
    }

    StateTreeComp = nullptr; // Clear the StateTree component reference
    AcquiredTarget = nullptr; // Clear the acquired target
}

// Broadcast handlers for perception events
void UPGAS_STE_Perception::HandleDamageStimulus(AActor* Actor, const FAIStimulus& Stimulus)
{
    AcquiredTarget = Actor;
    if (AIController.IsValid())
        AIController->AcquiredTarget = Actor; // Clear the AIController's target
    OnDamageStimulus(Actor, Stimulus);
    SendEvent(EPGAS_StateTreeEvent::Attack); // Send event to StateTree
}

void UPGAS_STE_Perception::HandleSightStimulus(AActor* Actor, const FAIStimulus& Stimulus)
{
    AcquiredTarget = Actor;
    if (AIController.IsValid())
        AIController->AcquiredTarget = Actor; // Clear the AIController's target
    OnSightStimulus(Actor, Stimulus);
    SendEvent(EPGAS_StateTreeEvent::Attack); // Send event to StateTree
}

void UPGAS_STE_Perception::HandleSightStimulusForgotten(AActor* Actor)
{
    AcquiredTarget = nullptr;
    if (AIController.IsValid())
        AIController->AcquiredTarget = nullptr; // Clear the AIController's target
    OnSightStimulusForgotten(Actor);
}

void UPGAS_STE_Perception::HandleHearingStimulus(AActor* Actor, const FAIStimulus& Stimulus)
{
    AcquiredTarget = Actor;
    if (AIController.IsValid())
        AIController->AcquiredTarget = Actor; // Clear the AIController's target
    OnHearingStimulus(Actor, Stimulus);
    SendEvent(EPGAS_StateTreeEvent::Attack); // Send event to StateTree
}

void UPGAS_STE_Perception::HandleHearingStimulusForgotten(AActor* Actor)
{
    AcquiredTarget = nullptr;
    if (AIController.IsValid())
        AIController->AcquiredTarget = nullptr; // Clear the AIController's target
    OnHearingStimulusForgotten(Actor);
}
