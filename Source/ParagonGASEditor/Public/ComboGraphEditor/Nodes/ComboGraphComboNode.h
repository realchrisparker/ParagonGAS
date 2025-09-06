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
 * ComboGraphComboNode.h
 * Defines the Combo Node for the Combat Combo Graph Editor.
 */

#pragma once

#include "CoreMinimal.h"
#include <ComboGraphEditor/Nodes/ComboGraphNodeBase.h>
#include "ComboGraphComboNode.generated.h"

 /**
  * Combo Node
  * Represents a step in a combat combo.
  * Has one input and two outputs: Success / Fail.
  */
UCLASS()
class PARAGONGASEDITOR_API UComboGraphComboNode : public UComboGraphNodeBase
{
    GENERATED_BODY()

public:

    // Title shown in editor
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
    {
        return NSLOCTEXT("ComboGraph", "ComboNodeTitle", "Combo");
    }

    // Node color
    virtual FLinearColor GetNodeTitleColor() const override
    {
        return FLinearColor(0.8f, 0.2f, 0.2f); // Red-ish
    }

    // Create default pins
    virtual void AllocateDefaultPins() override
    {
        // Input
        CreateFlowPin(EGPD_Input, TEXT("In"));

        // Outputs
        CreateFlowPin(EGPD_Output, TEXT("Success"));
        CreateFlowPin(EGPD_Output, TEXT("Fail"));
    }

    // Category in context menu
    virtual FText GetNodeCategory() const override
    {
        return NSLOCTEXT("ComboGraph", "NodeCategory", "Combat Combo");
    }
};
