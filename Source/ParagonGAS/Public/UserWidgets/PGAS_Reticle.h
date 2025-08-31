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
* Date: 7-6-2025
* =============================================================================
* PGAS_InGame_HUD.h
* This file defines the in-game HUD widget.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PGAS_Reticle.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent, DisplayName = "Aim or Lock On Reticle"))
class PARAGONGAS_API UPGAS_Reticle : public UUserWidget
{
    GENERATED_BODY()

public:
    /*
     * Functions
    */

    /*
     * Override BeginDestroy to clean up references
     * This is where you can clean up any references or perform final actions before the widget is removed
    */
    virtual void BeginDestroy() override
    {
        Super::BeginDestroy();
        // Nothing special yet, but you could clear refs here
    }

    /** Updates the screen position of the reticle image */
    UFUNCTION(BlueprintCallable, Category = "LockOn|UI")
    void SetScreenPosition(const FVector2D& ScreenPos)
    {
        FVector2D DesiredPos = ScreenPos;

        // Offset by half the widget size so it's centered
        FVector2D WidgetSize = GetDesiredSize();
        DesiredPos -= WidgetSize * 0.5f;

        // Correct for DPI scale
        if (GEngine && GEngine->GameViewport)
        {
            FVector2D ViewportSize;
            GEngine->GameViewport->GetViewportSize(ViewportSize);

            const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
            DesiredPos /= Scale;
        }

        SetPositionInViewport(DesiredPos, false); // false = absolute coords
    }

    /**
     * Properties
    */

    // Progress bars for health
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UImage> Reticle;
};
