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

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_GetRandomLocation.generated.h"

/**
 * StateTree instance data for the Get Random Location task.
 */
USTRUCT(BlueprintType, Category = "PGAS|RandomLocation",
    meta = (DisplayName = "Get Random Location Instance Data",
        Description = "Contains information about instance data.",
        ToolTip = "Contains information about a get random location instance data."))
    struct FGetRandomLocationTaskInstanceData
{
    GENERATED_BODY()

    /** The enemy character this task is associated with. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyCharacter> Actor;

    /** The AI controller managing the enemy character. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyAIController> AIController;

    // Radius in Unreal units for the search
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    float SearchRadius = 1000.f;

    // The result: a random, reachable location
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Output, meta = (Output))
    FVector RandomLocation = FVector::ZeroVector;
};


USTRUCT(Category = "PGAS",
    meta = (DisplayName = "Get Random Location",
        Description = "StateTree task to get a random reachable location within a radius around an actor.",
        ToolTip = "StateTree task to get a random reachable location within a radius around an actor.",
        Keywords = "Get Random Location, AI, Navigation, StateTree")
)
struct PARAGONGAS_API FPGAS_GetRandomLocation : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    // Tell the runtime what instance‐data type we’re using
    using FInstanceDataType = FGetRandomLocationTaskInstanceData;

    virtual const UStruct* GetInstanceDataType() const override { return FGetRandomLocationTaskInstanceData::StaticStruct(); }
    
    /*
     * Functions
    */
    
    /**
      * This function will find a random reachable location around the actor within the specified radius.
      * @param Context The execution context for the state tree.
      * @param Transition The transition result containing information about the state change.
    */
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        // Get the instance data for this task
        FGetRandomLocationTaskInstanceData& Data = Context.GetInstanceData(*this);

        // Safety checks
        if (!Data.Actor)
        {
            UE_LOG(LogTemp, Warning, TEXT("PGAS_GetRandomLocation: Actor is null."));
            return EStateTreeRunStatus::Failed;
        }

        UWorld* World = Data.Actor->GetWorld();
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

        FVector Origin = Data.Actor->GetActorLocation();
        FNavLocation NavLocation;

        const bool bFound = NavSys->GetRandomReachablePointInRadius(Origin, Data.SearchRadius, NavLocation);
        if (bFound)
        {
            Data.RandomLocation = NavLocation.Location;
            return EStateTreeRunStatus::Succeeded;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("PGAS_GetRandomLocation: Could not find random location."));
            return EStateTreeRunStatus::Failed;
        }
    }

#if WITH_EDITOR
    virtual FName GetIconName() const override { return FName("GenericPlay"); }
    virtual FColor GetIconColor() const override { return FColor::Silver; }
    virtual FText GetDescription(const FGuid& ID,
        FStateTreeDataView InstanceDataView,
        const IStateTreeBindingLookup& BindingLookup,
        EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override
    {
        // Try to get our instance data struct
        if (const FGetRandomLocationTaskInstanceData* Data = InstanceDataView.GetPtr<FGetRandomLocationTaskInstanceData>())
        {
            float Radius = Data->SearchRadius;

            return FText::Format(
                NSLOCTEXT("PGAS", "GetRandomLocationDescriptionDetailed", "Get random location around {0}, radius: {1} units"),
                FText::FromString("Actor"), // Placeholder for Actor name, if needed
                FText::AsNumber(Radius)
            );
        }

        // Fallback if data isn’t bound yet
        return NSLOCTEXT("PGAS", "GetRandomLocationTask_NoData", "Get Random Location (No Radius Defined)");
    }
#endif
};
