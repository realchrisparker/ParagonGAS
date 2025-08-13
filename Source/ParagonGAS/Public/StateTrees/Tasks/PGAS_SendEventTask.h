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
 * PGAS_SendEvent
 * A StateTree task that sends the StateTree an event.
 */

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeEditorPropertyBindings.h"
#include "StateTreePropertyBindings.h"
#include "GameplayTagContainer.h"
#include "Components/StateTreeAIComponent.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_SendEventTask.generated.h"


/**
  * StateTree instance data for the Send Event task.
  * This struct holds the context data for the task, such as the event to send.
 */
USTRUCT(BlueprintType)
struct FSendEventInstanceData
{
    GENERATED_BODY()

    /** The enemy character this task is associated with. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyCharacter> Actor;

    /** The AI controller managing the enemy character. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyAIController> AIController;

    /** The event to send */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    FGameplayTag EventTag;
};

/**
  * A StateTree task that sends an event to the StateTree.
*/
USTRUCT(category = "PGAS",
    meta = (
        DisplayName = "Send State Tree Event",
        ToolTip = "A task that sends an event to the StateTree.",
        Keywords = "Event, AI, Task")
)
struct PARAGONGAS_API FPGAS_SendEventTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    // Tell the runtime what instance‐data type we’re using
    using FInstanceDataType = FSendEventInstanceData;

    /**
     * Returns the instance data type for this task.
    */
    virtual const UStruct* GetInstanceDataType() const override { return FSendEventInstanceData::StaticStruct(); }

    /**
     * Called when entering the state
     * @param Context The execution context for the state tree
     * @param Transition The transition that triggered this state
     */
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        // Get the instance data for this task
        const FSendEventInstanceData& Data = Context.GetInstanceData(*this);

        // Check if the StateTree component and event tag are valid        
        if (Data.EventTag.IsValid())
        {
            APGAS_EnemyAIController* aiController = Cast<APGAS_EnemyAIController>(Context.GetOwner());
            if (aiController)
            {
                UStateTreeAIComponent* StateTreeComp = aiController->FindComponentByClass<UStateTreeAIComponent>();
                if (StateTreeComp)
                {
                    // Send the event to the StateTree
                    FStateTreeEvent Event(Data.EventTag);
                    StateTreeComp->SendStateTreeEvent(Event);
                }
            }
        }

        // Event sent successfully, so exit task.
        return EStateTreeRunStatus::Succeeded;
    }

#if WITH_EDITOR

    virtual FText GetDescription(const FGuid& ID,
        FStateTreeDataView InstanceDataView,
        const IStateTreeBindingLookup& BindingLookup,
        EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override
    {
        const FSendEventInstanceData* Data = InstanceDataView.GetPtr<FSendEventInstanceData>();
        if (!Data || !Data->EventTag.IsValid())
        {
            return NSLOCTEXT("PGAS", "SetFocusTask_NoData", "Send Event Task (No Tag)");
        }

        return FText::Format(
            NSLOCTEXT("PGAS", "SetFocusTask_Desc", "Send Event: {0}"),
            FText::FromName(Data->EventTag.GetTagName())
        );
    }
#endif
};
