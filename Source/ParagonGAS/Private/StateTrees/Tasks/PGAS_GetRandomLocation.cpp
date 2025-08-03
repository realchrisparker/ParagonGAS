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
 * Date: 7-29-2025
 * =============================================================================
 * StateTree task to get a random reachable location within a radius around an actor.
 */

#include "StateTrees/Tasks/PGAS_GetRandomLocation.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"

/**
 * This function will find a random reachable location around the actor within the specified radius.
 * @param Context The execution context for the state tree.
 * @param Transition The transition result containing information about the state change.
*/
EStateTreeRunStatus UPGAS_GetRandomLocation::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    // Safety checks
    if (!Actor)
    {
        UE_LOG(LogTemp, Warning, TEXT("PGAS_GetRandomLocation: Actor is null."));
        return EStateTreeRunStatus::Failed;
    }

    UWorld* World = Actor->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("PGAS_GetRandomLocation: World is null."));
        return EStateTreeRunStatus::Failed;
    }

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (!NavSys)
    {
        UE_LOG(LogTemp, Warning, TEXT("PGAS_GetRandomLocation: NavSys is null."));
        return EStateTreeRunStatus::Failed;
    }

    FVector Origin = Actor->GetActorLocation();
    FNavLocation NavLocation;

    const bool bFound = NavSys->GetRandomReachablePointInRadius(Origin, SearchRadius, NavLocation);
    if (bFound)
    {
        RandomLocation = NavLocation.Location;
        return EStateTreeRunStatus::Succeeded;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PGAS_GetRandomLocation: Could not find random location."));
        return EStateTreeRunStatus::Failed;
    }
}

#if WITH_EDITOR
FText UPGAS_GetRandomLocation::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
    const UPGAS_GetRandomLocation* Instance = InstanceDataView.GetPtr<UPGAS_GetRandomLocation>();
    float Radius = Instance ? Instance->SearchRadius : SearchRadius;

    // FString ActorName = Instance && Instance->Actor ? Instance->Actor->GetName() : TEXT("None");

    return FText::Format(
        NSLOCTEXT("PGAS", "GetRandomLocationDescriptionDetailed", "Get random location around {0}, radius: {1} units"),
        FText::FromString("Actor"), // Placeholder for Actor name, if needed
        FText::AsNumber(Radius)
    );
}
#endif
