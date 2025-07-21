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
 * Date: 7-13-2025
 * =============================================================================
 * PGAS_STT_HasGameplayTag
 * StateTree Task: Checks if an actor has a specified gameplay tag (generic for any tag)
 */

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "GameplayTagContainer.h"
#include "PGAS_STT_HasGameplayTag.generated.h"

USTRUCT(BlueprintType)
struct PARAGONGAS_API FHasGameplayTagTaskInstanceData
{
    GENERATED_BODY()

    // The tag to check for (generic, can be any tag)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
    AActor* Actor;

    // The tag to check for (generic, can be any tag)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    FGameplayTag TagToCheck;
};

USTRUCT(BlueprintType, meta=(DisplayName="Has Gameplay Tag Task"))
struct PARAGONGAS_API FPGAS_STT_HasGameplayTagTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY();

    using FInstanceDataType = FHasGameplayTagTaskInstanceData;

    UPROPERTY(BlueprintReadOnly, Category = "StateTree|Task|Result")
    bool bHasTag = false;

    virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

#if WITH_EDITOR
    virtual FName GetIconName() const override
    {
        return FName("StateTreeEditorStyle|Node.Task");
    }
    virtual FColor GetIconColor() const override
    {
        return UE::StateTree::Colors::Blue;
    }
#endif

    /**
     * Enter the state tree task. This is called when the task is first executed.
     * @param Context The execution context for the state tree.
     * @param Transition The transition that triggered this task.
     * @return The status of the task after entering.
    */
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

    /**
     * Tick the task. This is called every frame while the task is active.
     * @param Context The execution context for the state tree.
     * @param DeltaTime The time since the last tick.
     * @return The status of the task after ticking.
    */
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

    /**
     * Exit the state tree task. This is called when the task is completed or interrupted.
     * @param Context The execution context for the state tree.
     * @param Transition The transition that triggered the exit.
    */
    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

    /**
     * Called when the state tree task is completed.
     * @param Context The execution context for the state tree.
     * @param Transition The transition that triggered the completion.
    */
    virtual void StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates) const override;
};
