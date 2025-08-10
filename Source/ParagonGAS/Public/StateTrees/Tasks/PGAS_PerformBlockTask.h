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
 * Date: 8-9-2025
 * =============================================================================
 * StateTree task that makes the AI character perform a block using GAS.
 */

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "GameplayTagContainer.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_PerformBlockTask.generated.h"

 /**
  * StateTree instance data for the Perform Block task.
  */
USTRUCT(BlueprintType, Category = "PGAS",
    meta = (DisplayName = "Block Ability Instance Data",
        Description = "Contains information about instance data.",
        ToolTip = "Contains information about an block instance data.")
)
struct FPerformBlockTaskInstanceData
{
    GENERATED_BODY()

    /** The enemy character this task is associated with. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyCharacter> Actor;

    /** The AI controller managing the enemy character. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyAIController> AIController;

    /** The gameplay tag to trigger the block ability */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    FGameplayTag BlockAbilityTag;

    /** How long to hold the block before task finishes (seconds). Set to 0 for "until ability ends". */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task",
        meta = (ClampMin = "0.0",
            ClampMax = "60.0",
            DisplayName = "Block Duration",
            ToolTip = "How long to hold the block before task finishes (seconds). Set to 0 for 'until ability ends'.",
            Description = "How long to hold the block before task finishes (seconds). Set to 0 for 'until ability ends'."))
    float BlockDuration = 0.f;

    /** Internal timer */
    float ElapsedTime = 0.f;
};

USTRUCT(category = "PGAS",
    meta = (DisplayName = "Perform Block",
        Description = "A task that makes the AI character perform a block using GAS.",
        ToolTip = "A task that makes the AI character perform a block using GAS.")
)
struct PARAGONGAS_API FPGAS_BlockTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    // Tell the runtime what instance‐data type we’re using
    using FInstanceDataType = FPerformBlockTaskInstanceData;

    virtual const UStruct* GetInstanceDataType() const override { return FPerformBlockTaskInstanceData::StaticStruct(); }

    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        // Get the instance data for this task
        const FPerformBlockTaskInstanceData& Data = Context.GetInstanceData(*this);

        // Use the context-provided Actor directly
        if (!Data.Actor)
        {
            UE_LOG(LogTemp, Warning, TEXT("PerformBlockTask: Actor context is null."));
            return EStateTreeRunStatus::Failed;
        }

        if (Data.BlockAbilityTag.IsValid())
        {
            Data.Actor->ActivateAbilitiesWithTags(FGameplayTagContainer(Data.BlockAbilityTag), true);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("PGAS_BlockTask: Invalid BlockAbilityTag."));
            return EStateTreeRunStatus::Failed;
        }

        return EStateTreeRunStatus::Running;
    }

    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override
    {
        // Get the instance data for this task
        FPerformBlockTaskInstanceData& Data = Context.GetInstanceData(*this);

        Data.ElapsedTime += DeltaTime;

        // If BlockDuration > 0, finish after time expires
        if (Data.BlockDuration > 0.f && Data.ElapsedTime >= Data.BlockDuration)
        {
            return EStateTreeRunStatus::Succeeded;
        }

        // Optionally: Check if ability has ended here if you want to finish early
        return EStateTreeRunStatus::Running;
    }

    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        // Get the instance data for this task
        const FPerformBlockTaskInstanceData& Data = Context.GetInstanceData(*this);

        if (Data.Actor && Data.BlockAbilityTag.IsValid())
        {
            Data.Actor->CancelAbilitiesWithTags(FGameplayTagContainer(Data.BlockAbilityTag));
            // UE_LOG(LogTemp, Log, TEXT("PGAS_BlockTask: Canceled block ability for %s"), *Data.Actor->GetName());
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
        if (const FPerformBlockTaskInstanceData* Data = InstanceDataView.GetPtr<FPerformBlockTaskInstanceData>())
        {
            // Get block ability name
            FName BlockAbilityName = Data->BlockAbilityTag.GetTagName();

            return FText::Format(
                NSLOCTEXT("PGAS", "PerformBlockTaskDesc",
                    "Perform block with Tag {0}."),
                FText::FromName(BlockAbilityName)
            );
        }

        // Fallback if data isn’t bound yet
        return NSLOCTEXT("PGAS", "PerformBlockTask_NoData", "Perform Block with Tag");
    }
#endif
};
