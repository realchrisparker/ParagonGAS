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
 * Date: 8-3-2025
 * =============================================================================
 * PGAS_PlayMontageTask
 * A StateTree task that plays an animation montage on the context actor.
 */

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeEditorPropertyBindings.h"
#include "StateTreePropertyBindings.h"
#include <Characters/Base/PGAS_CharacterBase.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "PGAS_PlayMontageTask.generated.h"

/**
 * StateTree instance data for the Play Montage task.
 */
USTRUCT(BlueprintType)
struct FPlayMontageTaskInstanceData
{
    GENERATED_BODY()

    /** The enemy character this task is associated with. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyCharacter> Actor;

    /** The AI controller managing the enemy character. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyAIController> AIController;

    /** The montage to play. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (RequiredAssetDataTags = "AnimMontage"))
    TObjectPtr<UAnimMontage> Montage = nullptr;

    /** Play rate for the montage. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ClampMin = "0.01", ClampMax = "10.0"))
    float PlayRate = 1.0f;
};

/**
 * A StateTree task that plays an animation montage on the context actor.
 */
USTRUCT(category = "PGAS",
    meta = (DisplayName = "Play Montage",
        Description = "A task that plays an animation montage on the context actor.",
        ToolTip = "Play an animation montage on the context actor",
        Keywords = "Animation,Montage,Task")
)
struct PARAGONGAS_API FPGAS_PlayMontageTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    // Tell the runtime what instance‐data type we’re using
    using FInstanceDataType = FPlayMontageTaskInstanceData;

    /**
     * Returns the instance data type for this task.
    */
    virtual const UStruct* GetInstanceDataType() const override { return FPlayMontageTaskInstanceData::StaticStruct(); }

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
        const FPlayMontageTaskInstanceData& Data = Context.GetInstanceData(*this);
        if (ensure(Data.Actor && Data.Montage))
        {
            if (USkeletalMeshComponent* Mesh = Data.Actor->GetMesh())
            {
                if (UAnimInstance* AnimInst = Mesh->GetAnimInstance())
                {
                    AnimInst->Montage_Play(Data.Montage, Data.PlayRate);
                }
            }
        }

        // Keep running until the montage completes
        return EStateTreeRunStatus::Running;
    }

    /**
     * Called when state ticks
     * @param Context The execution context for the state tree
     * @param CompletionStatus The status of the completed state (Succeeded/Failed)
    */
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override
    {
        const FPlayMontageTaskInstanceData& Data = Context.GetInstanceData(*this);
        if (Data.Actor)
        {
            if (USkeletalMeshComponent* Mesh = Data.Actor->GetMesh())
            {
                if (UAnimInstance* AnimInst = Mesh->GetAnimInstance())
                {
                    if (AnimInst->Montage_IsPlaying(Data.Montage))
                    {
                        return EStateTreeRunStatus::Running;
                    }
                }
            }
        }

        // Montage has finished playing, exit the task
        return EStateTreeRunStatus::Succeeded;
    }

    /**
     * Called when exiting the state
     * @param Context The execution context for the state tree
     * @param Transition The transition that triggered this state
    */
    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        // Optionally stop the montage if still playing when exiting
        const FPlayMontageTaskInstanceData& Data = Context.GetInstanceData(*this);
        if (Data.Actor)
        {
            if (USkeletalMeshComponent* Mesh = Data.Actor->GetMesh())
            {
                if (UAnimInstance* AnimInst = Mesh->GetAnimInstance())
                {
                    if (AnimInst->Montage_IsPlaying(Data.Montage))
                    {
                        AnimInst->Montage_Stop(0.2f, Data.Montage);
                    }
                }
            }
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
        if (const FPlayMontageTaskInstanceData* Data = InstanceDataView.GetPtr<FPlayMontageTaskInstanceData>())
        {
            // Get montage name and play rate
            if (!Data->Montage)
            {
                return NSLOCTEXT("PGAS", "PlayMontageTask_NoMontage", "Play Montage Task (No Montage Set)");
            }

            return FText::Format(
                NSLOCTEXT("PGAS", "PlayMontageTaskDesc",
                    "Play montage {0} at a speed of {1}."),
                FText::FromString(Data->Montage->GetName()),
                FText::AsNumber(Data->PlayRate)
            );
        }
        
        // Fallback if data isn’t bound yet
        return NSLOCTEXT("PGAS", "PlayMontageTask_NoData", "Play Montage (No Data Bound)");
    }
#endif
};
