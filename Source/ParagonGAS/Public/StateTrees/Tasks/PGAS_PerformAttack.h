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
 * PGAS_PerformAttack
 * A StateTree task that selects and triggers a weighted, randomized, or sequential attack ability via GAS.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "GameplayTagContainer.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_PerformAttack.generated.h"


 /**
  * Modes for selecting which attack ability to fire
 */
UENUM(BlueprintType, Category = "PGAS|Attack",
    meta = (DisplayName = "Attack Selection Mode",
        Description = "Determines how the attack ability is selected.",
        ToolTip = "Determines how the attack ability is selected."))
    enum class EPGAS_AttackSelectMode : uint8
{
    Sequential UMETA(DisplayName = "Sequential"), // Selects abilities in order, looping back to the start when reaching the end
    Random     UMETA(DisplayName = "Random") // Selects a random ability from the list (weighted by their weights)
};

/**
 * Stores an ability tag and its weight for weighted selection
 */
USTRUCT(BlueprintType, Category = "PGAS|Attack",
    meta = (DisplayName = "Attack Ability Info",
        Description = "Contains information about an attack ability, including its tag and weight for selection.",
        ToolTip = "Contains information about an attack ability, including its tag and weight for selection."))
    struct PARAGONGAS_API FPGAS_AttackAbilityInfo
{
    GENERATED_BODY()

    /** Gameplay tag identifying the ability */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack",
        meta = (DisplayName = "Ability Tag",
            Description = "The Gameplay Tag that identifies the ability to be performed.",
            Tooltip = "The Gameplay Tag that identifies the ability to be performed."
            ))
    FGameplayTag AbilityTag;

    /** Weight for random selection (non-negative) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack",
        meta = (ClampMin = "0.0",
            DisplayName = "Selection Weight",
            Description = "Weight for random selection of this ability. Higher values increase the chance of selection.",
            Tooltip = "Higher values increase the chance of this ability being selected. Set to 0 to disable this ability."))
    float Weight = 1.0f;
};

/**
 * StateTree instance data for the Perform Attack task.
 */
USTRUCT(BlueprintType, Category = "PGAS|Attack",
    meta = (DisplayName = "Attack Ability Instance Data",
        Description = "Contains information about instance data.",
        ToolTip = "Contains information about an attack instance data."))
struct FPerformAttackTaskInstanceData
{
    GENERATED_BODY()

    /** The enemy character this task is associated with. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyCharacter> Actor;

    /** The AI controller managing the enemy character. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyAIController> AIController;

    /** Gameplay tags for the different attack abilities to choose from */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack",
        meta = (DisplayName = "Attack Ability Tags",
            Description = "The Gameplay Tags that identify the abilities to be performed.",
            Tooltip = "The Gameplay Tags that identify the abilities to be performed."))
    TArray<FPGAS_AttackAbilityInfo> AttackAbilities;

    /** Selection mode for choosing abilities */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack",
        meta = (DisplayName = "Selection Mode",
            Description = "The mode used to select the attack ability.",
            Tooltip = "The mode used to select the attack ability."))
    EPGAS_AttackSelectMode SelectionMode = EPGAS_AttackSelectMode::Random;
};

/**
 * A StateTree task that selects and triggers a weighted, randomized, or sequential attack ability via GAS.
*/
USTRUCT(category = "PGAS", meta = (DisplayName = "Perform Attack", ToolTip = "", Keywords = "Attack,GAS,Task"))
struct PARAGONGAS_API FPGAS_PerformAttackTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    // Tell the runtime what instance‐data type we’re using
    using FInstanceDataType = FPerformAttackTaskInstanceData;

    virtual const UStruct* GetInstanceDataType() const override { return FPerformAttackTaskInstanceData::StaticStruct(); }

    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        // Get the instance data for this task
        const FPerformAttackTaskInstanceData& Data = Context.GetInstanceData(*this);

        // Use the context-provided Actor directly
        if (!Data.Actor)
        {
            UE_LOG(LogTemp, Warning, TEXT("PerformAttackTask: Actor context is null."));
            return EStateTreeRunStatus::Failed;
        }

        // Check for tags
        const int32 Num = Data.AttackAbilities.Num();
        if (Num == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("PerformAttackTask: No attack abilities defined."));
            return EStateTreeRunStatus::Failed;
        }

        // Select an ability based on the selection mode
        int32 ChosenIndex = 0;
        if (Data.SelectionMode == EPGAS_AttackSelectMode::Random)
        {
            // Compute total weight
            float TotalWeight = 0.f;
            for (const FPGAS_AttackAbilityInfo& Info : Data.AttackAbilities)
            {
                TotalWeight += FMath::Max(0.f, Info.Weight);
            }
            if (TotalWeight <= 0.f)
            {
                UE_LOG(LogTemp, Warning, TEXT("PerformAttackTask: Total weight is zero."));
                return EStateTreeRunStatus::Failed;
            }

            // Pick a random weight
            float Rand = FMath::FRandRange(0.f, TotalWeight);
            float Accum = 0.f;
            for (int32 i = 0; i < Num; ++i)
            {
                Accum += FMath::Max(0.f, Data.AttackAbilities[i].Weight);
                if (Rand <= Accum)
                {
                    ChosenIndex = i;
                    break;
                }
            }
        }
        else // Sequential
        {
            static int32 SequentialIdx = 0;
            ChosenIndex = SequentialIdx++ % Num;
        }

        const FPGAS_AttackAbilityInfo& Chosen = Data.AttackAbilities[ChosenIndex];
        if (!Chosen.AbilityTag.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("PerformAttackTask: Selected ability tag is invalid."));
            return EStateTreeRunStatus::Failed;
        }

        FGameplayTagContainer Tags;
        Tags.AddTag(Chosen.AbilityTag);
        if (!Data.Actor->ActivateAbilitiesWithTags(Tags))
        {
            UE_LOG(LogTemp, Warning, TEXT("PerformAttackTask: Failed to activate ability %s."), *Chosen.AbilityTag.ToString());
            return EStateTreeRunStatus::Failed;
        }

        Data.Actor->SetIsAttacking(true);
        return EStateTreeRunStatus::Running;
    }

    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override
    {
        const FPerformAttackTaskInstanceData& Data = Context.GetInstanceData(*this);
        if (Data.Actor)
        {
            if (Data.Actor->IsAttacking())
            {
                return EStateTreeRunStatus::Running;
            }
        }

        // Montage has finished playing, exit the task
        return EStateTreeRunStatus::Succeeded;
    }

    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override
    {
        //
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
        if (const FPerformAttackTaskInstanceData* Data = InstanceDataView.GetPtr<FPerformAttackTaskInstanceData>())
        {
            // Get total number of attacks
            const int32 NumAttacks = Data->AttackAbilities.Num();

            // Get selection mode.
            const EPGAS_AttackSelectMode SelectionMode = Data->SelectionMode;

            return FText::Format(
                NSLOCTEXT("PGAS", "PerformAttackTaskDesc",
                    "Perform {0} attack(s) {1}."),
                FText::FromString(NumAttacks > 1 ? FString::Printf(TEXT("%d"), NumAttacks) : TEXT("1")),
                FText::FromString(SelectionMode == EPGAS_AttackSelectMode::Random ? "randomly" : "sequentially")
            );
        }

        // Fallback if data isn’t bound yet
        return NSLOCTEXT("PGAS", "PerformAttackTask_NoData", "Perform Attack (No Attacks Defined)");
    }
#endif
};
