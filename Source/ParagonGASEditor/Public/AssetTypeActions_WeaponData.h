#pragma once

#include "AssetTypeActions_Base.h"
#include "ParagonGAS/Public/Data/Assets/PGAS_WeaponDataAsset.h"
#include "ParagonGASEditorStyle.h"

/**
 * Customizes how UPGAS_WeaponDataAsset appears in the Content Browser
 */
class FAssetTypeActions_WeaponData : public FAssetTypeActions_Base
{
public:
    // Name that shows in the Content Browser
    virtual FText GetName() const override
    {
        return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_WeaponData", "Weapon Data Asset");
    }

    // Custom color (orange tint for visibility)
    virtual FColor GetTypeColor() const override
    {
        return FColor(255, 128, 0); // Orange
    }

    // Which class this action supports
    virtual UClass* GetSupportedClass() const override
    {
        return UPGAS_WeaponDataAsset::StaticClass();
    }

    // Which category in the Content Browser
    virtual uint32 GetCategories() override
    {
        return EAssetTypeCategories::Gameplay;
    }

    // 🔥 Here’s the magic — use our custom icon
    virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override
    {
        if (FParagonGASEditorStyle::Get().IsValid())
        {
            return FParagonGASEditorStyle::Get()->GetBrush("ClassIcon.WeaponDataAsset");
        }
        return nullptr; // fallback to default
    }
};
