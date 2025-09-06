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
 * Date: 8-25-2025
 * =============================================================================
 * Factory for creating new Weapon Data Assets.
 */

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AssetToolsModule.h"
#include "AssetTypeCategories.h"
#include <ParagonGAS/Public/Data/Assets/PGAS_CombatComboDataAsset.h>
#include "ComboGraphAssetFactory.generated.h"

extern EAssetTypeCategories::Type ParagonGASCategory;

UCLASS()
class PARAGONGASEDITOR_API UComboGraphAssetFactory : public UFactory
{
    GENERATED_BODY()

public:
    // Constructor
    UComboGraphAssetFactory()
    {
        bCreateNew = true;
        bEditAfterNew = true;
        SupportedClass = UPGAS_CombatComboGraph::StaticClass();
    }

    /**
     * Factory method to create a new instance of the supported class.
     * @param Class The class of the object to create.
     * @param InParent The parent object for the new object.
     * @param Name The name of the new object.
     * @param Flags The flags to apply to the new object.
     * @param Context The context in which the object is being created.
     * @param Warn The feedback context for warnings.
     * @return A new instance of the supported class.
     */
    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override
    {
        return NewObject<UPGAS_CombatComboGraph>(InParent, Class, Name, Flags | RF_Transactional);
    }

    /*
     * Get the menu categories for this factory.
     * @return The menu categories for this factory.
     */
    virtual uint32 GetMenuCategories() const override
    {
        return ParagonGASCategory;
    }

    /**
     * Determine whether this factory should be shown in the "New" menu.
     * @return True if the factory should be shown, false otherwise.
     */
    virtual bool ShouldShowInNewMenu() const override
    {
        return true;
    }

    /*
    * Get the display name for this factory.
    * @return The display name for this factory.
    */
    virtual FText GetDisplayName() const override
    {
        return NSLOCTEXT("ComboGraphAssetFactory", "DisplayName", "Combat Combo Graph");
    }

    /*
    * Get the tooltip for this factory.
    * @return The tooltip for this factory.
    */
    virtual FText GetToolTip() const override
    {
        return NSLOCTEXT("ComboGraphAssetFactory", "ToolTip", "Create a new PGAS Combat Combo Graph");
    }
};
