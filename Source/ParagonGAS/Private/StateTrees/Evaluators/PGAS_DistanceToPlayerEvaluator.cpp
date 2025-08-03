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
 * PGAS_DistanceToPlayerEvaluator.h
 * Evaluator to calculate and expose the distance from the Context Actor to the Player.
 */

#include <StateTrees/Evaluators/PGAS_DistanceToPlayerEvaluator.h>
#include "Kismet/GameplayStatics.h"

void UPGAS_DistanceToPlayerEvaluator::TreeStart(FStateTreeExecutionContext& Context)
{
    Super::TreeStart(Context);
}

void UPGAS_DistanceToPlayerEvaluator::Tick(FStateTreeExecutionContext& Context, float DeltaTime)
{
    Super::Tick(Context, DeltaTime);

    UWorld* World = Character->GetWorld();
    if (!World)
    {
        DistanceToPlayer = 0.f;
        return;
    }

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
    if (!PlayerPawn)
    {
        DistanceToPlayer = 0.f;
        return;
    }

    DistanceToPlayer = FVector::Dist(Character->GetActorLocation(), PlayerPawn->GetActorLocation());
}

void UPGAS_DistanceToPlayerEvaluator::TreeStop(FStateTreeExecutionContext& Context)
{
    Super::TreeStop(Context);
}