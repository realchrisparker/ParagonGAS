#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AssetToolsModule.h"
#include "AssetTypeCategories.h"
#include <Data/Assets/PGAS_WeaponDataAsset.h>
#include "WeaponDataAssetFactory.generated.h"

extern EAssetTypeCategories::Type ParagonGASCategory;

UCLASS()
class PARAGONGASEDITOR_API UWeaponDataAssetFactory : public UFactory
{
    GENERATED_BODY()

public:
    // Constructor
    UWeaponDataAssetFactory()
    {
        bCreateNew = true;
        bEditAfterNew = true;
        SupportedClass = UPGAS_WeaponDataAsset::StaticClass();
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
        return NewObject<UPGAS_WeaponDataAsset>(InParent, Class, Name, Flags | RF_Transactional);
    }

    /*
     * Get the menu categories for this factory.
     * @return The menu categories for this factory.
     */
    virtual uint32 GetMenuCategories() const override
    {
        return ParagonGASCategory; //EAssetTypeCategories::Misc;
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
        return NSLOCTEXT("WeaponDataAssetFactory", "DisplayName", "Weapon Data Asset");
    }

    /*
    * Get the tooltip for this factory.
    * @return The tooltip for this factory.
    */
    virtual FText GetToolTip() const override
    {
        return NSLOCTEXT("WeaponDataAssetFactory", "ToolTip", "Create a new PGAS Weapon Data Asset");
    }
};
