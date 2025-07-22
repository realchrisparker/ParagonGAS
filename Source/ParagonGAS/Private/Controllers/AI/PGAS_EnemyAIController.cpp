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
 * Custom AI Controller for Enemy Characters (StateTree-ready)
 */

#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "GameFramework/Pawn.h"
#include "StateTreePropertyRef.h"
#include "StateTreePropertyBindings.h"

APGAS_EnemyAIController::APGAS_EnemyAIController()
{
    bAttachToPawn = true; // Attach controller to pawn

    // Set up the State Tree AI Component for managing enemy AI behavior.
    // This component handles the AI behavior for the enemy character.
    StateTreeAIComponent = CreateDefaultSubobject<UPGAS_StateTreeAIComponent>(TEXT("State Tree AI Component"));
    
    // Create Perception Component
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));

    // Add sight sense
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1000.0f;
    SightConfig->LoseSightRadius = 1500.0f;
    SightConfig->PeripheralVisionAngleDegrees = 45.0f;
    SightConfig->SetMaxAge(5.f); // How long the perception lasts when player is not seen anymore
    SightConfig->PointOfViewBackwardOffset = 250.0f; // How far behind the character to check for sight (Peripheral vision)
    SightConfig->NearClippingRadius = 175.0f; // How close the character can be to still be seen (Peripheral vision)
    SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.0f; // How far to check for last seen location (Turned off)
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    PerceptionComponent->ConfigureSense(*SightConfig); // Assign Sight as the sense
    PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass()); // Set Sight as the dominant sense

    // Add hearing
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = 1200.f;
    HearingConfig->SetMaxAge(3.f);
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

    PerceptionComponent->ConfigureSense(*HearingConfig);

    // Add damage perception
    DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
    PerceptionComponent->ConfigureSense(*DamageConfig);

    // Bind to the perception delegates so we can receive events from the perception system
    // PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &APGAS_EnemyAIController::OnTargetPerceptionUpdated);
    // PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &APGAS_EnemyAIController::OnTargetPerceptionForgotten);
}

/**
 * Called when the game starts or when spawned.
 * This is where we can initialize any AI-specific logic.
*/
void APGAS_EnemyAIController::BeginPlay()
{
    Super::BeginPlay();
}

/**
 * Called when the controller possesses a pawn.
 * This is where we can initialize the AI behavior and perception.
 */
void APGAS_EnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UE_LOG(LogTemp, Warning, TEXT("APGAS_EnemyAIController::OnPossess - Possessing Pawn: %s"), *InPawn->GetName());

    OwningCharacter = Cast<APGAS_EnemyCharacter>(InPawn);
    if (!OwningCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("APGAS_EnemyAIController::OnPossess - InPawn is not a PGAS_EnemyCharacter!"));
        return;
    }
}

/**
 * Called when the controller is unpossessed from a pawn.
 * This is where we can clean up or stop AI behavior.
 */
void APGAS_EnemyAIController::OnUnPossess()
{
    Super::OnUnPossess();

    // UE_LOG(LogTemp, Warning, TEXT("APGAS_EnemyAIController::OnUnPossess - Unpossessing Pawn"));

    // Clean up or stop StateTree logic
    OwningCharacter = nullptr; // Clear the reference to the character
}


/* ============================================================
 * Callbacks for perception updates
 * These handle the perception events and can be customized for specific behavior.
*/

/**
 * Called when the perception system updates the target's perception.
 * This is where we handle the perception of the target actor.
 * @param Actor The actor that was perceived.
 * @param Stimulus The stimulus data containing information about the perception.
*/
void APGAS_EnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // Validate the Actor pointer
    if (!Actor)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnTargetPerceptionUpdated called with null Actor!"));
        return;
    }

    // Static IDs for sense types (only initialized once)
    static const FAISenseID SightID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
    static const FAISenseID HearingID = UAISense::GetSenseID(UAISense_Hearing::StaticClass());
    static const FAISenseID DamageID = UAISense::GetSenseID(UAISense_Damage::StaticClass());

    // Determine the sense type
    FString SenseName;
    if (Stimulus.Type == SightID)
        SenseName = TEXT("Sight");
    else if (Stimulus.Type == HearingID)
        SenseName = TEXT("Hearing");
    else if (Stimulus.Type == DamageID)
        SenseName = TEXT("Damage");
    else {
        SenseName = TEXT("Unknown");
    }

    // Validate and cast to your specific target class ONCE
    APGAS_PlayerCharacter* SensedPlayer = Cast<APGAS_PlayerCharacter>(Actor);
    if (!SensedPlayer)
    {
        UE_LOG(LogTemp, Verbose, TEXT("Actor %s is not a PGAS_PlayerCharacter."), *GetNameSafe(Actor));
        return;
    }

    // Update targeting info+
    // StateTreeAIComponent->SetAcquiredTargetActorParameter(SensedPlayer); // Set the target actor in the StateTree
    // StateTreeAIComponent->SetPointOfInterestParameter(SensedPlayer->GetActorLocation());
    // StateTreeAIComponent->SetIsAcquiredTargetVisibleParameter(Stimulus.WasSuccessfullySensed()); // Update visibility state

    // 6. Perception handling - use switch for clarity and perf
    if (Stimulus.Type == DamageID)
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            HandleDamagePerception(SensedPlayer, Stimulus);
        }
    }
    else if (Stimulus.Type == SightID)
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            HandleSightPerception(SensedPlayer, Stimulus);
        }
        else
        {
            HandleLostSightPerception(SensedPlayer, Stimulus);
        }
    }
    else if (Stimulus.Type == HearingID)
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            HandleHearingPerception(SensedPlayer, Stimulus);
        }
        else
        {
            HandleLostHearingPerception(SensedPlayer, Stimulus);
        }
    }
}

/**
 * Called when the perception system forgets an actor.
 * This is where we handle the case where the perception of an actor is forgotten.
 * @param Actor The actor that was forgotten.
*/
void APGAS_EnemyAIController::OnTargetPerceptionForgotten(AActor* Actor)
{
    UE_LOG(LogTemp, Warning, TEXT("PERCEPTION FORGOTTEN: All perception of actor %s has expired or been explicitly forgotten."), *GetNameSafe(Actor));

    // Handle the case where the perception of an actor is forgotten
    // StateTreeAIComponent->SetMotivationParameter(StateTreeAIComponent->GetDefaultMotivationParameter()); // Update StateTree parameter
    // StateTreeAIComponent->SetIsAcquiredTargetVisibleParameter(false); // Update visibility state
}


void APGAS_EnemyAIController::HandleDamagePerception(APGAS_PlayerCharacter* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("Enemy took DAMAGE from: %s"), *Actor->GetName());

    // Implement damage handling logic here
    if (!Actor)
    {
        return;
    }

    SetFocus(Actor); // Set focus on the perceived actor
    // StateTreeAIComponent->SetMotivationParameter(FGameplayTag::RequestGameplayTag(FName("Character.Motivation.Attack"))); // Update StateTree parameter
    // StateTreeAIComponent->SendEvent(FGameplayTag::RequestGameplayTag(FName("StateTree.Event.DamagePerceived"))); // Send the event
}

void APGAS_EnemyAIController::HandleSightPerception(APGAS_PlayerCharacter* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("Perceived (Sight): %s"), *Actor->GetName());

    if (!Actor)
    {
        return;
    }

    SetFocus(Actor); // Set focus on the perceived actor
    // StateTreeAIComponent->SetMotivationParameter(FGameplayTag::RequestGameplayTag(FName("Character.Motivation.Attack"))); // Update StateTree parameter
    // StateTreeAIComponent->SendEvent(FGameplayTag::RequestGameplayTag(FName("StateTree.Event.SightPerceived"))); // Send the event
}

void APGAS_EnemyAIController::HandleLostSightPerception(APGAS_PlayerCharacter* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("Lost sight of: %s"), *Actor->GetName());

    if (!Actor)
    {
        return;
    }

    ClearFocus(EAIFocusPriority::Gameplay); // Clear focus when the actor is no longer seen
    // StateTreeAIComponent->SetMotivationParameter(StateTreeAIComponent->GetDefaultMotivationParameter()); // Update StateTree parameter
}

void APGAS_EnemyAIController::HandleHearingPerception(APGAS_PlayerCharacter* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("Heard: %s"), *Actor->GetName());

    if (!Actor)
    {
        return;
    }
    
    SetFocus(Actor); // Set focus on the perceived actor
    // StateTreeAIComponent->SetMotivationParameter(FGameplayTag::RequestGameplayTag(FName("Character.Motivation.Attack"))); // Update StateTree parameter
    // StateTreeAIComponent->SendEvent(FGameplayTag::RequestGameplayTag(FName("StateTree.Event.HearingPerceived"))); // Send the event
}

void APGAS_EnemyAIController::HandleLostHearingPerception(APGAS_PlayerCharacter* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("Lost hearing of: %s"), *Actor->GetName());

    if (!Actor)
    {
        return;
    }

    ClearFocus(EAIFocusPriority::Gameplay); // Clear focus when the actor is no longer heard
    // StateTreeAIComponent->SetMotivationParameter(StateTreeAIComponent->GetDefaultMotivationParameter()); // Update StateTree parameter
}
