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
 * ComboGraphNodeBase.h
 * Base node class for Combat Combo Graph editor.
 */

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "ComboGraphNodeBase.generated.h"


// Forward declare schema to avoid circular include
class UComboGraphSchema;


/**
  * Base class for all nodes in the Combat Combo Graph.
  */
UCLASS(Abstract)
class PARAGONGASEDITOR_API UComboGraphNodeBase : public UEdGraphNode
{
    GENERATED_BODY()

public:

    // Static category name for flow pins (same as Schema)
    static const FName PC_Flow;

    // Title shown in the graph
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
    {
        return NSLOCTEXT("ComboGraph", "BaseNode", "Combo Node Base");
    }

    // Category shown in context menu
    virtual FText GetNodeCategory() const
    {
        return NSLOCTEXT("ComboGraph", "NodeCategory", "Combat Combo");
    }

    // Override: default color
    virtual FLinearColor GetNodeTitleColor() const override
    {
        return FLinearColor(0.2f, 0.2f, 0.2f); // dark gray for base
    }

    // Override: disallow direct renaming
    virtual bool CanUserDeleteNode() const override { return true; }
    virtual bool CanDuplicateNode() const override { return true; }

protected:

    /**
     * Helper to create a flow pin (exec style) on this node.
     * @param Direction The direction of the pin (input or output).
     * @param PinName The name of the pin.
     * @return The created pin.
     */
    UEdGraphPin* CreateFlowPin(EEdGraphPinDirection Direction, const FName& PinName)
    {
        FEdGraphPinType PinType(
            PC_Flow, // We only need the symbol
            NAME_None,
            nullptr,
            EPinContainerType::None,
            false,
            FEdGraphTerminalType()
        );

        return CreatePin(Direction, PinType, PinName);
    }
};

// Define the static constant here (one definition)
inline const FName UComboGraphNodeBase::PC_Flow(TEXT("Flow"));
