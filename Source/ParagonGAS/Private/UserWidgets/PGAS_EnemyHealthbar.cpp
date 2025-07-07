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
* PGAS_EnemyHealthbar.h
* This file defines the enemy health bar widget in the game.
*/

#include "UserWidgets/PGAS_EnemyHealthbar.h"

// Fired when the widget is about to be destroyed
// This is where you can clean up any references or perform final actions before the widget is removed
void UPGAS_EnemyHealthbar::BeginDestroy()
{
    Super::BeginDestroy();

    // Clean up widget references if necessary
    Healthbar = nullptr;
}

/**
 * Update the health value displayed on the HUD
 * @param NewValue The new health value to display
 * @param MaxValue The maximum health value to normalize against
*/
void UPGAS_EnemyHealthbar::UpdateHealthValue(float NewValue, float MaxValue) const
{
    Healthbar->SetPercent(NewValue / MaxValue); // Assuming NewValue is a percentage
}

void UPGAS_EnemyHealthbar::UpdateHealthBarVisibility(bool bIsAlive)
{
    if (Healthbar)
    {
        Healthbar->SetVisibility(bIsAlive ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}
