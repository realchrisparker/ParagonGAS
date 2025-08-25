#include "ParagonGASEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr<FSlateStyleSet> FParagonGASEditorStyle::StyleSet = nullptr;

void FParagonGASEditorStyle::Initialize()
{
    if (StyleSet.IsValid())
    {
        return; // already registered, skip
    }

    StyleSet = MakeShareable(new FSlateStyleSet("ParagonGASEditorStyle"));
    FString ContentDir = FPaths::ProjectContentDir() / TEXT("Resources");
    StyleSet->SetContentRoot(ContentDir);

    const FVector2D Icon16(16.f, 16.f);
    const FVector2D Icon64(64.f, 64.f);


    FString IconPath = StyleSet->RootToContentDir(TEXT("T_sword.png"));
    UE_LOG(LogTemp, Warning, TEXT("WeaponDataAsset Icon Path: %s"), *IconPath);

    StyleSet->Set("ClassIcon.PGAS_WeaponDataAsset", new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("T_sword.png")), Icon16));
    StyleSet->Set("ClassThumbnail.PGAS_WeaponDataAsset", new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("T_sword.png")), Icon64));

    // ✅ Register only once
    FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FParagonGASEditorStyle::Shutdown()
{
    if (StyleSet.IsValid())
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
        ensure(StyleSet.IsUnique());
        StyleSet.Reset();
    }
}

TSharedPtr<ISlateStyle> FParagonGASEditorStyle::Get()
{
    return StyleSet;
}

FName FParagonGASEditorStyle::GetStyleSetName()
{
    return "ParagonGASEditorStyle";
}
