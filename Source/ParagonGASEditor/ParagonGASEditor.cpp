#include "ParagonGASEditor.h"
#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"
#include "ClassIconFinder.h"
#include "Styling/SlateIconFinder.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "ParagonGASEditorStyle.h"
#include "WeaponDataAsset/AssetTypeActions/AssetTypeActions_WeaponData.h"
#include "ComboGraphEditor/AssetTypeActions/AssetTypeActions_ComboGraph.h"


IMPLEMENT_MODULE(FParagonGASEditorModule, ParagonGASEditor)


// Store as TSharedRef, not TSharedPtr
TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetActions;

EAssetTypeCategories::Type ParagonGASCategory;


void FParagonGASEditorModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("ParagonGASEditor module started"));

    FParagonGASEditorStyle::Initialize();

    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    // Register a new top-level category
    ParagonGASCategory = AssetTools.RegisterAdvancedAssetCategory(
        FName(TEXT("ParagonGAS")),      // Internal name
        FText::FromString("Paragon GAS") // Displayed name in Add New
    );

    // Register asset actions
    {
		TSharedRef<IAssetTypeActions> Action = MakeShareable(new FAssetTypeActions_WeaponData());
		AssetTools.RegisterAssetTypeActions(Action);
		RegisteredAssetActions.Add(Action);
    }

	{
		TSharedRef<IAssetTypeActions> Action = MakeShareable(new FAssetTypeActions_ComboGraph());
		AssetTools.RegisterAssetTypeActions(Action);
		RegisteredAssetActions.Add(Action);
	}
}

void FParagonGASEditorModule::ShutdownModule()
{
    // Unregister when shutting down
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        for (auto& Action : RegisteredAssetActions)
        {
            AssetTools.UnregisterAssetTypeActions(Action);
        }
    }

    RegisteredAssetActions.Empty();

    FParagonGASEditorStyle::Shutdown();
}

#undef LOCTEXT_NAMESPACE