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
 * Factory for creating new Enemy Character Blueprints with APGAS_EnemyCharacter as the parent.
 */

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AssetTypeCategories.h"
#include "Kismet2/KismetEditorUtilities.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include "EnemyCharacterFactory.generated.h"

extern EAssetTypeCategories::Type ParagonGASCategory;

UCLASS()
class PARAGONGASEDITOR_API UEnemyCharacterFactory : public UFactory
{
    GENERATED_BODY()

public:
    // Constructor
    UEnemyCharacterFactory()
    {
        bCreateNew = true;
        bEditAfterNew = true;
        SupportedClass = UBlueprint::StaticClass();
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
        // Create a Blueprint with APGAS_EnemyCharacter as the parent
        UClass* ParentClass = APGAS_EnemyCharacter::StaticClass();

        return FKismetEditorUtilities::CreateBlueprint(
            ParentClass,
            InParent,
            Name,
            BPTYPE_Normal,              // Normal Blueprint
            UBlueprint::StaticClass(),  // Blueprint asset type
            UBlueprintGeneratedClass::StaticClass(),
            FName("EnemyCharacterFactory")
        );
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
        return NSLOCTEXT("EnemyCharacterFactory", "DisplayName", "Enemy Character Blueprint");
    }

    /*
    * Get the tooltip for this factory.
    * @return The tooltip for this factory.
    */
    virtual FText GetToolTip() const override
    {
        return NSLOCTEXT("EnemyCharacterFactory", "ToolTip", "Create a new PGAS Enemy Character");
    }
};
