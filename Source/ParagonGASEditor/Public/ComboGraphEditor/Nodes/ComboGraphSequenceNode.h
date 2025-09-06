/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * Author: Christopher D. Parker
 * Date: 9-4-2025
 * =============================================================================
 * ComboGraphSequenceNode.h
 * Defines the Sequence Node for the Combat Combo Graph Editor.
 */

#pragma once

#include "CoreMinimal.h"
#include <ParagonGASEditor/Public/ComboGraphEditor/Nodes/ComboGraphNodeBase.h>
#include "ComboGraphSequenceNode.generated.h"


/**
  * Sequence Node
  * Acts as an entry/start point for a combo chain.
  * Has a single output pin.
  */
UCLASS()
class PARAGONGASEDITOR_API UComboGraphSequenceNode : public UComboGraphNodeBase
{
    GENERATED_BODY()

public:

    // Title shown in editor
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
    {
        return NSLOCTEXT("ComboGraph", "SequenceNodeTitle", "Sequence");
    }

    // Node color
    virtual FLinearColor GetNodeTitleColor() const override
    {
        return FLinearColor(0.1f, 0.3f, 0.8f); // Blue-ish
    }

    // Create default pins
    virtual void AllocateDefaultPins() override
    {
        // Sequence nodes only have one output pin
        CreateFlowPin(EGPD_Output, TEXT("Out"));
    }

    // Category in context menu
    virtual FText GetNodeCategory() const override
    {
        return NSLOCTEXT("ComboGraph", "NodeCategory", "Combat Combo");
    }
};
