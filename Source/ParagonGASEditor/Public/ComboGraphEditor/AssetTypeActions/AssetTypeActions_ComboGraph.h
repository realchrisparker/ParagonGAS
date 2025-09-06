#pragma once

#include "AssetTypeActions_Base.h"
#include "ParagonGASEditorStyle.h"
#include "ComboGraphEditor/ComboGraphEditor.h"
#include "ParagonGAS/Public/Data/Assets/PGAS_CombatComboDataAsset.h"

extern EAssetTypeCategories::Type ParagonGASCategory;

/**
 * Customizes how UPGAS_CombatComboGraph appears in the Content Browser
 */
class FAssetTypeActions_ComboGraph : public FAssetTypeActions_Base
{
public:
    virtual FText GetName() const override
    {
        return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ComboGraph", "Combat Combo Graph");
    }

    virtual FColor GetTypeColor() const override
    {
        return FColor(255, 128, 0);
    }

    virtual UClass* GetSupportedClass() const override
    {
        return UPGAS_CombatComboGraph::StaticClass();
    }

    virtual uint32 GetCategories() override
    {
        return ParagonGASCategory;
    }

    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override
    {
        const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
            ? EToolkitMode::WorldCentric
            : EToolkitMode::Standalone;

        for (UObject* Obj : InObjects)
        {
            if (UPGAS_CombatComboGraph* Asset = Cast<UPGAS_CombatComboGraph>(Obj))
            {
                TSharedRef<FComboGraphEditor> NewEditor = MakeShareable(new FComboGraphEditor());
                NewEditor->InitComboGraphEditor(Mode, EditWithinLevelEditor, Asset);
            }
        }
    }
};
