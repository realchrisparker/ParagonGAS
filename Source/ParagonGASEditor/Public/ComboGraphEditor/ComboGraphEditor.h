#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Widgets/Docking/SDockTab.h"
#include "GraphEditor.h"
#include "EdGraph/EdGraph.h"
#include <ComboGraphEditor/Graph/ComboGraph.h>
#include <ComboGraphEditor/Schema/ComboGraphSchema.h>
#include <ParagonGAS/Public/Data/Assets/PGAS_CombatComboDataAsset.h>
#include <ComboGraphEditor/Utils/ComboGraphAssetEditor.h>


/**
 * Editor toolkit for editing Combat Combo Graph assets.
 */
class FComboGraphEditor : public FAssetEditorToolkit, public FNotifyHook
{
public:
    /** Initialize the editor for the given asset */
    void InitComboGraphEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UPGAS_CombatComboGraph* InAsset)
    {
        check(InAsset);
        EditingAsset = InAsset;

        // Use existing graph or create new one
        if (!EditingAsset->EditorGraph)
        {
            // Create new graph
            EditingAsset->EditorGraph = NewObject<UComboGraph>(EditingAsset, UComboGraph::StaticClass(), NAME_None, RF_Transactional);
        }
        EditingGraph = EditingAsset->EditorGraph;

        // Load from Sequences if the asset already has data
        if (EditingAsset->Sequences.Num() > 0)
        {
            // LoadFromAsset(); // rebuild nodes from saved data
        }
        else {
            const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("Standalone_ComboGraphEditor_Layout_v1")
                ->AddArea
                (
                    FTabManager::NewPrimaryArea()
                    ->SetOrientation(Orient_Vertical)
                    ->Split
                    (
                        FTabManager::NewStack()
                        ->AddTab("GraphEditor", ETabState::OpenedTab)->SetHideTabWell(true)
                    )
                );

            // This sets up TabManager internally
            InitAssetEditor(Mode, InitToolkitHost, FName("ComboGraphEditorApp"), Layout, true, true, InAsset);
            RegenerateMenusAndToolbars();
        }
    }

    /** Override save to push graph state into the asset before saving */
    virtual void SaveAsset_Execute() override
    {
        if (EditingAsset && EditingGraph)
        {
            ComboGraphEditorUtils::RebuildFromGraph(EditingAsset, EditingGraph);
            EditingAsset->MarkPackageDirty();
        }

        FAssetEditorToolkit::SaveAsset_Execute();
    }

    // --- FAssetEditorToolkit interface ---
    virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override
    {
        WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(
            NSLOCTEXT("ComboGraphEditor", "WorkspaceMenu_ComboGraph", "Combat Combo Graph"));

        FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

        InTabManager->RegisterTabSpawner("GraphEditor", FOnSpawnTab::CreateSP(this, &FComboGraphEditor::SpawnGraphTab))
            .SetDisplayName(NSLOCTEXT("ComboGraphEditor", "GraphTab", "Graph"))
            .SetGroup(WorkspaceMenuCategory.ToSharedRef());
    }

    virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override
    {
        FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
        InTabManager->UnregisterTabSpawner("GraphEditor");
    }

    virtual FName GetToolkitFName() const override { return FName("ComboGraphEditor"); }
    virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("ComboGraphEditor", "AppLabel", "Combat Combo Graph Editor"); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("ComboGraph"); }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.3f, 0.1f, 0.6f); }

private:
    /*
    * Functions
    */

    /** Spawns the graph tab */
    TSharedRef<SDockTab> SpawnGraphTab(const FSpawnTabArgs& Args)
    {
        FGraphAppearanceInfo AppearanceInfo;
        AppearanceInfo.CornerText = NSLOCTEXT("ComboGraphEditor", "AppearanceCornerText", "Combat Combo Graph");

        SGraphEditor::FGraphEditorEvents Events;

        GraphEditorWidget = SNew(SGraphEditor)
            .GraphToEdit(EditingGraph)
            .Appearance(AppearanceInfo)
            .GraphEvents(Events);

        return SNew(SDockTab)
            .Label(NSLOCTEXT("ComboGraphEditor", "GraphTabLabel", "Graph"))
            [
                GraphEditorWidget.ToSharedRef()
            ];
    }

    /** Save the current state to the asset */
    // void SaveToAsset()
    // {
    //     if (!EditingAsset || !EditingGraph) return;

    //     // Clear old sequences
    //     EditingAsset->Sequences.Empty();

    //     // Walk all nodes in the editor graph
    //     for (UEdGraphNode* Node : EditingGraph->Nodes)
    //     {
    //         if (UComboGraphSequenceNode* SeqNode = Cast<UComboGraphSequenceNode>(Node))
    //         {
    //             FPGAS_CombatComboSequence NewSequence;
    //             NewSequence.SequenceName = *SeqNode->GetName();
    //             NewSequence.RootStepIndex = INDEX_NONE;

    //             // Find the root combo node connected to this sequence
    //             UComboGraphComboNode* RootComboNode = nullptr;
    //             if (UEdGraphPin* OutPin = SeqNode->FindPin(TEXT("Out")))
    //             {
    //                 if (OutPin->LinkedTo.Num() > 0)
    //                 {
    //                     RootComboNode = Cast<UComboGraphComboNode>(OutPin->LinkedTo[0]->GetOwningNode());
    //                 }
    //             }

    //             // If no combos connected, just save the sequence name
    //             if (!RootComboNode)
    //             {
    //                 EditingAsset->Sequences.Add(NewSequence);
    //                 continue;
    //             }

    //             // Traverse the graph of Combo nodes (depth-first)
    //             TMap<UComboGraphComboNode*, int32> NodeToIndex;
    //             TArray<UComboGraphComboNode*> Stack;
    //             Stack.Push(RootComboNode);

    //             while (Stack.Num() > 0)
    //             {
    //                 UComboGraphComboNode* ComboNode = Stack.Pop();
    //                 if (!ComboNode || NodeToIndex.Contains(ComboNode))
    //                     continue;

    //                 int32 StepIndex = NewSequence.Combos.Num();
    //                 NodeToIndex.Add(ComboNode, StepIndex);

    //                 FPGAS_CombatCombo NewStep;
    //                 NewStep.Name = *ComboNode->GetName();

    //                 // (Later: copy TriggerTag, AbilityTag, MontageToPlay from node props)

    //                 // Push linked nodes for traversal
    //                 if (UEdGraphPin* SuccessPin = ComboNode->FindPin(TEXT("Success")))
    //                 {
    //                     if (SuccessPin->LinkedTo.Num() > 0)
    //                     {
    //                         if (UComboGraphComboNode* NextNode = Cast<UComboGraphComboNode>(SuccessPin->LinkedTo[0]->GetOwningNode()))
    //                         {
    //                             Stack.Push(NextNode);
    //                         }
    //                     }
    //                 }
    //                 if (UEdGraphPin* FailPin = ComboNode->FindPin(TEXT("Fail")))
    //                 {
    //                     if (FailPin->LinkedTo.Num() > 0)
    //                     {
    //                         if (UComboGraphComboNode* NextNode = Cast<UComboGraphComboNode>(FailPin->LinkedTo[0]->GetOwningNode()))
    //                         {
    //                             Stack.Push(NextNode);
    //                         }
    //                     }
    //                 }

    //                 NewSequence.Combos.Add(NewStep);
    //             }

    //             // Resolve Success/Fail links into array indices
    //             for (auto& Pair : NodeToIndex)
    //             {
    //                 UComboGraphComboNode* ComboNode = Pair.Key;
    //                 int32 StepIndex = Pair.Value;

    //                 FPGAS_CombatCombo& Step = NewSequence.Combos[StepIndex];

    //                 if (UEdGraphPin* SuccessPin = ComboNode->FindPin(TEXT("Success")))
    //                 {
    //                     if (SuccessPin->LinkedTo.Num() > 0)
    //                     {
    //                         if (UComboGraphComboNode* NextNode = Cast<UComboGraphComboNode>(SuccessPin->LinkedTo[0]->GetOwningNode()))
    //                         {
    //                             if (int32* IndexPtr = NodeToIndex.Find(NextNode))
    //                             {
    //                                 Step.SuccessStepIndex = *IndexPtr;
    //                             }
    //                         }
    //                     }
    //                 }
    //                 if (UEdGraphPin* FailPin = ComboNode->FindPin(TEXT("Fail")))
    //                 {
    //                     if (FailPin->LinkedTo.Num() > 0)
    //                     {
    //                         if (UComboGraphComboNode* NextNode = Cast<UComboGraphComboNode>(FailPin->LinkedTo[0]->GetOwningNode()))
    //                         {
    //                             if (int32* IndexPtr = NodeToIndex.Find(NextNode))
    //                             {
    //                                 Step.FailStepIndex = *IndexPtr;
    //                             }
    //                         }
    //                     }
    //                 }
    //             }

    //             // Assign real root index
    //             if (NodeToIndex.Contains(RootComboNode))
    //             {
    //                 NewSequence.RootStepIndex = NodeToIndex[RootComboNode];
    //             }

    //             // Save sequence
    //             EditingAsset->Sequences.Add(NewSequence);
    //         }
    //     }

    //     // Mark dirty so Unreal knows asset has changed
    //     EditingAsset->MarkPackageDirty();
    // }


    /** Rebuild the graph from the asset's saved sequences */
    void LoadFromAsset()
    {
        if (!EditingAsset || !EditingGraph) return;

        // // Clear any existing nodes
        // while (EditingGraph->Nodes.Num() > 0)
        // {
        //     UEdGraphNode* Node = EditingGraph->Nodes[0];
        //     EditingGraph->RemoveNode(Node);
        // }

        // // For each sequence in the asset, create a SequenceNode
        // for (int32 SeqIndex = 0; SeqIndex < EditingAsset->Sequences.Num(); ++SeqIndex)
        // {
        //     const FPGAS_CombatComboSequence& Sequence = EditingAsset->Sequences[SeqIndex];

        //     FGraphNodeCreator<UComboGraphSequenceNode> SeqCreator(*EditingGraph);
        //     UComboGraphSequenceNode* SeqNode = SeqCreator.CreateNode();
        //     SeqNode->NodePosX = SeqIndex * 400; // offset placement for visibility
        //     SeqNode->NodePosY = 0;
        //     SeqCreator.Finalize();

        //     // TODO: store Sequence.SequenceName in the node if needed

        //     // Now add combo steps for this sequence
        //     for (int32 StepIndex = 0; StepIndex < Sequence.Combos.Num(); ++StepIndex)
        //     {
        //         const FPGAS_CombatCombo& Step = Sequence.Combos[StepIndex];

        //         FGraphNodeCreator<UComboGraphComboNode> ComboCreator(*EditingGraph);
        //         UComboGraphComboNode* ComboNode = ComboCreator.CreateNode();
        //         ComboNode->NodePosX = SeqIndex * 400;
        //         ComboNode->NodePosY = 150 + StepIndex * 120;
        //         ComboCreator.Finalize();

        //         // TODO: set node properties from Step (e.g. Name, TriggerTag, AbilityTag, MontageToPlay)

        //         // TODO: recreate links between pins using Step.SuccessStepIndex / FailStepIndex
        //     }
        // }
    }

    /*
    * Properties
    */
    
    /** The asset we’re editing */
    UPGAS_CombatComboGraph* EditingAsset = nullptr;

    /** The actual EdGraph */
    UComboGraph* EditingGraph = nullptr;

    /** Slate Graph Editor widget */
    TSharedPtr<SGraphEditor> GraphEditorWidget;
};
