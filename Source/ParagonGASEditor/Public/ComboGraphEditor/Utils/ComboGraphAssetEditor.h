#pragma once

#include "CoreMinimal.h"
#include <ParagonGAS/Public/Data/Assets/PGAS_CombatComboDataAsset.h>
#include "ComboGraphEditor/Graph/ComboGraph.h"
#include "ComboGraphEditor/Nodes/ComboGraphSequenceNode.h"
#include "ComboGraphEditor/Nodes/ComboGraphComboNode.h"


class UComboGraph;
class UPGAS_CombatComboGraph;


/** Editor helper to rebuild runtime data from editor graph */
namespace ComboGraphEditorUtils
{
    void RebuildFromGraph(UPGAS_CombatComboGraph* Asset, UComboGraph* Graph)
    {
        if (!Asset || !Graph) return;

        // Asset->Sequences.Empty();

        for (UEdGraphNode* Node : Graph->Nodes)
        {
            if (UComboGraphSequenceNode* SeqNode = Cast<UComboGraphSequenceNode>(Node))
            {
                FPGAS_CombatComboSequence NewSequence;
                NewSequence.SequenceName = *SeqNode->GetName();
                NewSequence.RootStepIndex = INDEX_NONE;

                // TODO: traverse linked ComboNodes, build Combos[], set RootStepIndex, etc.

                Asset->Sequences.Add(NewSequence);
            }
        }
    }
}
