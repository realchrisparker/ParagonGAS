#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FParagonGASEditorStyle
{
public:
    static void Initialize();
    static void Shutdown();
    static TSharedPtr<ISlateStyle> Get();
    static FName GetStyleSetName();

private:
    static TSharedPtr<FSlateStyleSet> StyleSet;
};
