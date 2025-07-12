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
    * Date: 6-28-2025
    * =============================================================================
*/


#include "Game/PGAS_HUD.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

/**
 * Constructor for the PGAS_HUD class.
 * Initializes the HUD and sets up any necessary properties or defaults.
*/
APGAS_HUD::APGAS_HUD()
{
    // Constructor logic can be added here if needed.
}

/**
 * BeginPlay is called when the game starts or when spawned.
 * Initialize HUD elements here.
*/
void APGAS_HUD::BeginPlay()
{
    Super::BeginPlay();

    // Only create on locally controlled PlayerController
    if (APlayerController* PC = GetOwningPlayerController())
    {
        // Static load: safer than ConstructorHelpers for game widgets
        TSubclassOf<UUserWidget> HUDWidget = LoadClass<UUserWidget>(nullptr, TEXT("/Game/_Game/UI/WBP_InGame_HUD.WBP_InGame_HUD_C"));
        if (HUDWidget)
        {
            // Create the HUD widget and add it to the viewport
            InGameHUDWidget = CreateWidget<UPGAS_InGame_HUD>(PC, HUDWidget);
            if (InGameHUDWidget)
            {
                InGameHUDWidget->AddToViewport(); // Add the widget to the viewport
                InGameHUDWidget->SetVisibility(ESlateVisibility::Visible); // Ensure it's visible
            }
        }
    }
}

/*
* IHUD Interface Implementation
* These functions update the HUD elements with the new values.
*/

/**
 * Update the health bar UI with the given percentage.
 * @param NewPercentage - The new percentage to update the health bar.
 * @return The new percentage after update (may be clamped or processed).
*/
float APGAS_HUD::UpdateHealthBar(float NewValue, float MaxValue) const
{
    if (InGameHUDWidget)
    {
        InGameHUDWidget->UpdateHealthValue(NewValue, MaxValue);
    }
        
    return 0.0f;
}

/**
 * Update the stamina bar UI with the given percentage.
 * @param NewPercentage - The new percentage to update the stamina bar.
 * @return The new percentage after update.
*/
float APGAS_HUD::UpdateStaminaBar(float NewValue, float MaxValue) const
{
    if (InGameHUDWidget)
    {
        InGameHUDWidget->UpdateStaminaValue(NewValue, MaxValue);
    }
    
    return 0.0f;
}

/**
 * Update the mana bar UI with the given percentage.
 * @param NewPercentage - The new percentage to update the mana bar.
 * @return The new percentage after update.
*/
float APGAS_HUD::UpdateAdrenalineBar(float NewValue, float MaxValue) const
{
    if (InGameHUDWidget)
    {
        InGameHUDWidget->UpdateAdrenalineValue(NewValue, MaxValue);
    }

    return 0.0f;
}

/**
 * Update the experience bar UI with the given percentage.
 * @param NewPercentage - The new percentage to update the experience bar.
 * @return The new percentage after update.
*/
float APGAS_HUD::UpdateExperienceBar(float NewValue, float MaxValue) const
{
    if (InGameHUDWidget)
    {
        // InGameHUDWidget->UpdateExperienceValue(NewValue, MaxValue);
    }

    return 0.0f;
}
