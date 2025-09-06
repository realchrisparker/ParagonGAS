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
 * ComboGraphSchema.h
 * Defines the schema (rules) for the Combat Combo Graph Editor.
 */

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include <ComboGraphEditor/Nodes/ComboGraphSequenceNode.h>
#include <ComboGraphEditor/Nodes/ComboGraphComboNode.h>
#include "ComboGraphSchema.generated.h"


 /**
  * Schema for Combat Combo Graphs.
  * Handles node creation, connection rules, and context menus.
  */
UCLASS()
class PARAGONGASEDITOR_API UComboGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()

public:

    /** Pin categories */
    static const FName PC_Flow; // Declaration only

    /** Return all possible actions when right-clicking in the graph */
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override
    {
        // Add Sequence Node action
        {
            TSharedPtr<FEdGraphSchemaAction_NewNode> NewAction =
                MakeShared<FEdGraphSchemaAction_NewNode>(
                    NSLOCTEXT("ComboGraph", "NodeCategory", "Combat Combo"),
                    NSLOCTEXT("ComboGraph", "AddSequenceNode", "Add Sequence Node"),
                    NSLOCTEXT("ComboGraph", "AddSequenceNodeTooltip", "Create a new Sequence node"),
                    0);

            UComboGraphSequenceNode* TemplateNode = NewObject<UComboGraphSequenceNode>(ContextMenuBuilder.OwnerOfTemporaries);
            NewAction->NodeTemplate = Cast<UEdGraphNode>(TemplateNode);

            ContextMenuBuilder.AddAction(NewAction);
        }

        // Add Combo Node action
        {
            TSharedPtr<FEdGraphSchemaAction_NewNode> NewAction =
                MakeShared<FEdGraphSchemaAction_NewNode>(
                    NSLOCTEXT("ComboGraph", "NodeCategory", "Combat Combo"),
                    NSLOCTEXT("ComboGraph", "AddComboNode", "Add Combo Node"),
                    NSLOCTEXT("ComboGraph", "AddComboNodeTooltip", "Create a new Combo node"),
                    0);

            UComboGraphComboNode* TemplateNode = NewObject<UComboGraphComboNode>(ContextMenuBuilder.OwnerOfTemporaries);
            NewAction->NodeTemplate = Cast<UEdGraphNode>(TemplateNode);

            ContextMenuBuilder.AddAction(NewAction);
        }
    }

    /** Validate whether a connection between two pins is allowed */
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override
    {
        // Disallow same node connections
        if (A->GetOwningNode() == B->GetOwningNode())
        {
            return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect pins on the same node"));
        }

        // Disallow same direction connections (both inputs or both outputs)
        if (A->Direction == B->Direction)
        {
            return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins must be in opposite directions"));
        }

        // Enforce only flow pins
        if (A->PinType.PinCategory != PC_Flow || B->PinType.PinCategory != PC_Flow)
        {
            return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Only flow pins can be connected"));
        }

        // Extra guard: only allow one connection into an input pin
        if (B->Direction == EGPD_Input && B->LinkedTo.Num() > 0)
        {
            return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Input pin already connected"));
        }
        if (A->Direction == EGPD_Input && A->LinkedTo.Num() > 0)
        {
            return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Input pin already connected"));
        }

        // Passed all checks
        return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("Connect nodes"));
    }

    /** Provide default node category for context menu */
    virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override
    {
        return FLinearColor::White;
    }

    virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override
    {
        // Use the GraphNodeCreator helper to spawn our Sequence node
        FGraphNodeCreator<UComboGraphSequenceNode> NodeCreator(Graph);
        UComboGraphSequenceNode* SequenceNode = NodeCreator.CreateNode();
        NodeCreator.Finalize();

        // Place node at origin
        SequenceNode->NodePosX = 0;
        SequenceNode->NodePosY = 0;
    }
};

// Inline definition in the header
inline const FName UComboGraphSchema::PC_Flow(TEXT("Flow"));
