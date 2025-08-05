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
 * Date: 8-2-2025
 * =============================================================================
 * PGAS_SetFocusTask
 * A StateTree task that sets the focus on a actor.
 */

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeEditorPropertyBindings.h"
#include "StateTreePropertyBindings.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_SetFocusTask.generated.h"


 /**
   * StateTree instance data for the Set Focus task.
  * This struct holds the context data for the task, such as the actor to focus on.
 */
USTRUCT(BlueprintType)
struct FInstanceData
{
    GENERATED_BODY()

    /** The enemy character this task is associated with. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyCharacter> Actor;

    /** The AI controller managing the enemy character. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyAIController> AIController;

    /** The actor to focus on */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    TObjectPtr<AActor> FocusActor = nullptr;

    /** Whether to set focus on the actor */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task", meta = (DisplayName = "Set Focus", ToolTip = "If true, the AI will set focus on the specified actor. If false, it will clear focus."))
    bool bSetFocusOnActor = true;
};

/**
  * A StateTree task that sets the focus on a actor.
*/
USTRUCT(category = "PGAS",
    meta = (
        DisplayName = "Set Focus Task",
        ToolTip = "A task that sets the focus on a specified actor.",
        Keywords = "Focus, AI, Task"))
struct PARAGONGAS_API FPGAS_SetFocusTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    // Tell the runtime what instance‐data type we’re using
    using FInstanceDataType = FInstanceData;

    /**
     * Returns the instance data type for this task.
    */
    virtual const UStruct* GetInstanceDataType() const override { return FInstanceData::StaticStruct(); }

    /**
     * Called when entering the state
     * @param Context The execution context for the state tree
     * @param Transition The transition that triggered this state
    */
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        const FInstanceData& Data = Context.GetInstanceData(*this);
        if (ensure(Data.FocusActor))
        {
            if (AAIController* AI = Cast<AAIController>(Context.GetOwner()))
            {
                AI->SetFocus(Data.FocusActor);
            }
        }

        // Keep the task alive so that TickState will be invoked
        return EStateTreeRunStatus::Running;
    }

    /**
     * Called when state ticks
     * @param Context The execution context for the state tree
     * @param CompletionStatus The status of the completed state (Succeeded/Failed)
    */
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override
    {
        return EStateTreeRunStatus::Succeeded;
    }

#if WITH_EDITOR

    virtual FName GetIconName() const override { return FName("GenericPlay"); }
    virtual FColor GetIconColor() const override { return FColor::Silver; }
    virtual FText GetDescription(const FGuid& ID,
        FStateTreeDataView InstanceDataView,
        const IStateTreeBindingLookup& BindingLookup,
        EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override
    {
        const FInstanceData* Data = InstanceDataView.GetPtr<FInstanceData>();
        if (!Data)
        {
            return NSLOCTEXT("PGAS", "SetFocusTask_NoData", "Set Focus Task");
        }

        // Build the binding path for FocusActor
        const FPropertyBindingPath InstancePath(ID, GET_MEMBER_NAME_CHECKED(FInstanceData, FocusActor));
        const FPropertyBindingPath* SourcePath = BindingLookup.GetPropertyBindingSource(InstancePath);

        FString BoundName;
        if (SourcePath && SourcePath->NumSegments() > 0)
        {
            for (int32 SegmentIndex = 0; SegmentIndex < SourcePath->NumSegments(); ++SegmentIndex)
            {
                if (SegmentIndex > 0)
                {
                    BoundName.AppendChar(TEXT('.'));
                }
                const FPropertyBindingPathSegment& Segment = SourcePath->GetSegment(SegmentIndex);
                BoundName += Segment.GetName().ToString();
            }
        }
        else
        {
            BoundName = TEXT("[Bound Parameter]");
        }

        return FText::Format(
            NSLOCTEXT("PGAS", "SetFocusTaskDesc", "Set Focus to Actor {0}"),
            FText::FromString(BoundName)
        );
    }
#endif
};
