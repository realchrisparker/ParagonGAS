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
 * UPGAS_CompareGameplayTags
 * StateTree Condition: Compares two Gameplay Tags and returns true/false based on the selected comparison type.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "GameplayTagContainer.h"
#include "PGAS_CompareGameplayTags.generated.h"


UCLASS(Blueprintable, BlueprintType, meta = (DisplayName = "Compare Gameplay Tags"), Category = "PGAS")
class PARAGONGAS_API UPGAS_CompareGameplayTags : public UStateTreeConditionBlueprintBase
{
    GENERATED_BODY()

public:
    /*
     * Functions
    */
    
    /**
     * Tests the condition based on the specified comparison type.
     * @param Context The execution context for the StateTree.
     * @return True if the condition is met, false otherwise.
    */
    virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
    /**
     * Gets the description of the condition for editor display.
     * @param ID The unique identifier for the node.
     * @param InstanceDataView The data view for the instance.
     * @param BindingLookup The binding lookup for the state tree.
     * @param Formatting The formatting options for the description.
     * @return The formatted description text.
    */
    virtual FText GetDescription(
        const FGuid& ID,
        FStateTreeDataView InstanceDataView,
        const IStateTreeBindingLookup& BindingLookup,
        EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif

    /*
     * Properties
    */
    
    /** The first gameplay tag to compare. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    FGameplayTag ConditionTag;

    /** The second gameplay tag to compare. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    FGameplayTag InputTag;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    bool ExactMatch = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
    bool Invert = false;
};
