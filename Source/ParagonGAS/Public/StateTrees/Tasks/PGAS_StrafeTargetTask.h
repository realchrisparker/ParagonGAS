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
 * Date: 8-4-2025
 * =============================================================================
 * PGAS_StrafeTargetTask
 * A StateTree task that makes the enemy character strafe around its focus target at a given radius and speed.
 */

#pragma once

#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeEditorPropertyBindings.h"
#include "StateTreePropertyBindings.h"
#include "NavigationSystem.h"
#include "NavigationData.h"
#include "Navigation/PathFollowingComponent.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_StrafeTargetTask.generated.h"


 /**
  * StateTree instance data for the Set Focus task.
  * This struct holds the context data for the task, such as the actor to focus on.
 */
USTRUCT(BlueprintType)
struct FStrafeTargetInstanceData
{
    GENERATED_BODY()

    /** The enemy character this task is associated with. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyCharacter> Actor;

    /** The AI controller managing the enemy character. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyAIController> AIController;

    /** Radius to maintain around the target */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (DisplayName = "Radius"))
    float StrafeRadius = 300.f;

    /** Maximum angular deviation (degrees) applied each tick */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ClampMin = "0.0", DisplayName = "Angular Deviation"))
    float StrafeDeviationDegrees = 15.f;

    /** Speed at which to move around the target */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (DisplayName = "Speed"))
    float StrafeSpeed = 200.f;

    /** How long to strafe before succeeding (seconds) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta= (DisplayName = "Duration"))
    float StrafeDuration = 2.f;

    /** Maximum random deviation in duration (seconds) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ClampMin = "0.0", DisplayName = "Duration Deviation"))
    float StrafeDurationDeviation = 0.f;

    /** How long we’ve been strafing */
    float ElapsedTime = 0.f;

    /** Current angle around the target (degrees) */
    float CurrentAngle = 0.f;
};


/**
  * A StateTree task that makes the enemy character strafe around its focus target at a given radius and speed.
  * This task is designed to be used in AI behavior trees to create dynamic movement patterns.
*/
USTRUCT(Category = "PGAS",
    meta = (DisplayName = "Strafe Target",
        Description = "Strafe around the target at a specified radius and speed.",
        Keywords = "Strafe,Target,Movement,AI",
        ToolTip = "A task that makes the enemy character strafe around its focus target at a given radius and speed.")
)
struct PARAGONGAS_API FPGAS_StrafeTargetTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    // Tell the runtime what instance‐data type we’re using
    using FInstanceDataType = FStrafeTargetInstanceData;

    /**
     * Returns the instance data type for this task.
    */
    virtual const UStruct* GetInstanceDataType() const override { return FStrafeTargetInstanceData::StaticStruct(); }

    /*
    * Functions
    */

    /**
     * Called when entering the state
     * @param Context The execution context for the state tree
     * @param Transition The transition that triggered this state
    */
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        // Get the instance data for this task
        FStrafeTargetInstanceData& Data = Context.GetInstanceData(*this);

        Data.CurrentAngle = 0.f; // Reset angle to start strafing from the beginning
        Data.ElapsedTime = 0.f; // Reset elapsed time

        // Pick a random actual duration within deviation
        Data.StrafeDuration = FMath::FRandRange(
            Data.StrafeDuration - Data.StrafeDurationDeviation,
            Data.StrafeDuration + Data.StrafeDurationDeviation
        );

        return EStateTreeRunStatus::Running;
    }

    /**
     * Called when state ticks
     * @param Context The execution context for the state tree
     * @param CompletionStatus The status of the completed state (Succeeded/Failed)
    */
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override
    {
        // Get the instance data for this task
        FStrafeTargetInstanceData& Data = Context.GetInstanceData(*this);

        // Accumulate elapsed time and finish when duration reached
        Data.ElapsedTime += DeltaTime;
        if (Data.ElapsedTime >= Data.StrafeDuration)
        {
            return EStateTreeRunStatus::Succeeded;
        }

        // Ensure we have a valid actor
        if (!Data.Actor)
            return EStateTreeRunStatus::Failed;

        // Ensure we have a valid AI controller
        if (!Data.AIController)
            return EStateTreeRunStatus::Failed;

        // Ensure we have a target to strafe around
        AActor* Target = Data.AIController->AcquiredTarget;
        if (!Target)
            return EStateTreeRunStatus::Failed;

        // Advance base angle based on speed
        float Circumference = 2 * PI * Data.StrafeRadius;
        float RotationRate = (Data.StrafeSpeed / Circumference) * 360.f; // degrees per second
        Data.CurrentAngle += RotationRate * DeltaTime;

        // Add a random angular deviation
        float Deviation = FMath::FRandRange(-Data.StrafeDeviationDegrees, Data.StrafeDeviationDegrees);
        float TotalAngle = Data.CurrentAngle + Deviation;
        float Rad = FMath::DegreesToRadians(TotalAngle);

        FVector DesiredLocation = Target->GetActorLocation()
            + FVector(FMath::Cos(Rad) * Data.StrafeRadius,
                FMath::Sin(Rad) * Data.StrafeRadius, 0.f);

        // Create a move request to the desired location
        FAIMoveRequest MoveReq;
        MoveReq.SetGoalLocation(DesiredLocation);
        MoveReq.SetAcceptanceRadius(5.f);

        // Move the AI controller towards the desired location
        Data.AIController->MoveTo(MoveReq);

        // Let task continue running
        return EStateTreeRunStatus::Running;
    }

    /**
     * Called when exiting the state
     * @param Context The execution context for the state tree
     * @param Transition The transition that triggered this state
    */
    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
    {
        // Get the instance data for this task
        FStrafeTargetInstanceData& Data = Context.GetInstanceData(*this);

        if (Data.AIController)
        {
            Data.AIController->StopMovement();
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
        if (const FStrafeTargetInstanceData* Data = InstanceDataView.GetPtr<FStrafeTargetInstanceData>())
        {
            return FText::Format(
                NSLOCTEXT("PGAS", "StrafeTargetTaskDesc",
                    "Strafe around the target at radius {0}, speed {1}, for {2}±{3} seconds, with ±{4}° deviation."),
                FText::AsNumber(Data->StrafeRadius),
                FText::AsNumber(Data->StrafeSpeed),
                FText::AsNumber(Data->StrafeDuration),
                FText::AsNumber(Data->StrafeDurationDeviation),
                FText::AsNumber(Data->StrafeDeviationDegrees)
            );
        }
        // Fallback if data isn’t bound yet
        return NSLOCTEXT("PGAS", "StrafeTargetTask_NoData", "Strafe around the target");
    }

#endif
};
