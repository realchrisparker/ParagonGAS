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
        // TSubclassOf<UUserWidget> HUDWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/_Game/UI/WBP_HUD.WBP_HUD_C"));
        // if (HUDWidgetClass)
        // {
        //     // Create the HUD widget and add it to the viewport
        //     HUDWidget = CreateWidget<UPGAS_HUDUserWidgetBase>(PC, HUDWidgetClass);
        //     if (HUDWidget)
        //     {
        //         HUDWidget->AddToViewport(); // Add the widget to the viewport
        //         HUDWidget->SetVisibility(ESlateVisibility::Visible); // Ensure it's visible
        //     }
        // }
    }
}

/**
 * Update the health bar UI with the given percentage.
 * @param NewPercentage - The new percentage to update the health bar.
 * @return The new percentage after update (may be clamped or processed).
*/
// float APGAS_HUD::UpdateHealthBar(float NewPercentage) const
// {
//     if (HUDWidget)
//     {
//         HUDWidget->HealthOrb->UpdateOrbValue(NewPercentage);
//     }
        
//     return 0.0f;
// }

// /**
//  * Update the stamina bar UI with the given percentage.
//  * @param NewPercentage - The new percentage to update the stamina bar.
//  * @return The new percentage after update.
// */
// float APGAS_HUD::UpdateStaminaBar(float NewPercentage) const
// {
//     if (HUDWidget)
//     {
//         HUDWidget->StaminaOrb->UpdateOrbValue(NewPercentage);
//     }
    
//     return 0.0f;
// }

// /**
//  * Update the mana bar UI with the given percentage.
//  * @param NewPercentage - The new percentage to update the mana bar.
//  * @return The new percentage after update.
// */
// float APGAS_HUD::UpdateAdrenalineBar(float NewPercentage) const
// {
//     if (HUDWidget)
//     {
//         HUDWidget->AdrenalineOrb->UpdateOrbValue(NewPercentage);
//     }

//     return 0.0f;
// }

// /**
//  * Update the experience bar UI with the given percentage.
//  * @param NewPercentage - The new percentage to update the experience bar.
//  * @return The new percentage after update.
// */
// float APGAS_HUD::UpdateExperienceBar(float NewPercentage) const
// {
//     return 0.0f;
// }
