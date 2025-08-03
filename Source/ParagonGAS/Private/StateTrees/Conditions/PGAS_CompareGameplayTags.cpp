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

#include "StateTrees/COnditions/PGAS_CompareGameplayTags.h"
#include "StateTreeExecutionContext.h"


 /**
 * Tests the condition based on the specified comparison type.
 * @param Context The execution context for the StateTree.
 * @return True if the condition is met, false otherwise.
*/
bool UPGAS_CompareGameplayTags::TestCondition(FStateTreeExecutionContext& Context) const
{
    bool bMatch = false;

    if (ExactMatch)
    {
        bMatch = InputTag == ConditionTag;
    }
    else
    {
        bMatch = InputTag.MatchesTag(ConditionTag);
    }

    return Invert ? !bMatch : bMatch;
}

#if WITH_EDITOR

/**
 * Gets the description of the condition for editor display.
 * @param ID The unique identifier for the node.
 * @param InstanceDataView The data view for the instance.
 * @param BindingLookup The binding lookup for the state tree.
 * @param Formatting The formatting options for the description.
 * @return The formatted description text.
*/
FText UPGAS_CompareGameplayTags::GetDescription(
    const FGuid& ID,
    FStateTreeDataView InstanceDataView,
    const IStateTreeBindingLookup& BindingLookup,
    EStateTreeNodeFormatting Formatting) const
{
    // Retrieve instance data for the current node (ourself)
    const UPGAS_CompareGameplayTags* Instance = InstanceDataView.GetPtr<UPGAS_CompareGameplayTags>();
    if (!Instance)
    {
        return NSLOCTEXT("PGAS", "CompareGameplayTagsNoInstance", "Compare [Invalid Instance]");
    }

    FText MatchType = Instance->ExactMatch ? FText::FromString("==") : FText::FromString("Matches");
    FText InvertText = Instance->Invert ? FText::FromString(" (Inverted)") : FText::GetEmpty();
    FText InputTagText = FText::FromName(Instance->InputTag.GetTagName());
    FText ConditionTagText = FText::FromName(Instance->ConditionTag.GetTagName());

    return FText::Format(
        NSLOCTEXT("PGAS", "CompareGameplayTagsDescription", "Compare {0} {1} {2}{3}"),
        InputTagText, MatchType, ConditionTagText, InvertText
    );
}

#endif
