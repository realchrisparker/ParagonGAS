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
 * PGAS_UpdateMovementSpeed.cpp
 */

#include "StateTrees/Tasks/PGAS_UpdateMovementSpeed.h"
#include "Characters/Base/PGAS_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

EStateTreeRunStatus UPGAS_UpdateMovementSpeed::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    Super::EnterState(Context, Transition);

    // Safety checks
    if (!Actor)
    {
        UE_LOG(LogTemp, Warning, TEXT("PGAS_GetRandomLocation: Actor is null."));
        return EStateTreeRunStatus::Failed;
    }

    // Get the character movement component
    if (UCharacterMovementComponent* MoveComp = Actor->GetCharacterMovement())
    {
        MoveComp->MaxWalkSpeed = TargetMaxWalkSpeed; // Set the desired MaxWalkSpeed
        return EStateTreeRunStatus::Succeeded;   // Successfully set the speed
    }

    UE_LOG(LogTemp, Warning, TEXT("PGAS_UpdateMovementSpeed: Character or MovementComponent not valid"));
    return EStateTreeRunStatus::Failed;
}

#if WITH_EDITOR
FText UPGAS_UpdateMovementSpeed::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
    return FText::Format(
        NSLOCTEXT("PGAS", "UpdateMovementSpeed", "Set Actor Max Walk Speed to {0}"),
        FText::AsNumber(TargetMaxWalkSpeed)
    );
}
#endif /*WITH_EDITOR*/