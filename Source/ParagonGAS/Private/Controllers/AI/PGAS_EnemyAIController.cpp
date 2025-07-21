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

#include "Controllers/AI/PGAS_EnemyAIController.h"
#include "Components/StateTreeComponent.h"
#include "GameFramework/Pawn.h"


APGAS_EnemyAIController::APGAS_EnemyAIController()
{
    UE_LOG(LogTemp, Warning, TEXT("APGAS_EnemyAIController::Constructor - Initializing Enemy AI Controller"));
    bAttachToPawn = true; // Attach controller to pawn

    // Create Perception Component
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));

    // Create and configure Sight sense
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 2000.0f;
    SightConfig->LoseSightRadius = 2200.0f;
    SightConfig->PeripheralVisionAngleDegrees = 70.0f;
    SightConfig->SetMaxAge(5.f);
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    // Assign Sight as the sense
    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

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


    // Bind to the perception updated delegate
    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &APGAS_EnemyAIController::OnTargetPerceptionUpdated);
}

void APGAS_EnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("APGAS_EnemyAIController::BeginPlay - Controller initialized for %s"), *GetName());
}

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

void APGAS_EnemyAIController::OnUnPossess()
{
    Super::OnUnPossess();

    UE_LOG(LogTemp, Warning, TEXT("APGAS_EnemyAIController::OnUnPossess - Unpossessing Pawn"));

    // Clean up or stop StateTree logic
    OwningCharacter = nullptr; // Clear the reference to the character
}

void APGAS_EnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    FString Sense = Stimulus.Type.Name.ToString();

    if (Sense == TEXT("Damage"))
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            UE_LOG(LogTemp, Warning, TEXT("Enemy took DAMAGE from: %s"), *Actor->GetName());
            // TODO: React to being damaged (alert, flee, counterattack, etc.)
        }
        // No "else": Damage is an instant event, not a persistent stimulus like Sight
    }
    else if (Sense == TEXT("Sight"))
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            UE_LOG(LogTemp, Warning, TEXT("Perceived (Sight): %s"), *Actor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Lost sight of: %s"), *Actor->GetName());
        }
    }
    else if (Sense == TEXT("Hearing"))
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            UE_LOG(LogTemp, Warning, TEXT("Heard: %s"), *Actor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Lost hearing of: %s"), *Actor->GetName());
        }
    }
}

