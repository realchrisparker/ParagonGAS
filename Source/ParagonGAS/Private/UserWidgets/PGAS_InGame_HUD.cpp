// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/PGAS_InGame_HUD.h"


// Fired when the widget is about to be destroyed
// This is where you can clean up any references or perform final actions before the widget is removed
void UPGAS_InGame_HUD::BeginDestroy()
{
    Super::BeginDestroy();

    // Clean up widget references if necessary
    Healthbar = nullptr;
    Staminabar = nullptr;
    Adrenalinebar = nullptr;
}

/**
 * Update the health value displayed on the HUD
 * @param NewValue The new health value to display
 * @param MaxValue The maximum health value to normalize against
*/
void UPGAS_InGame_HUD::UpdateHealthValue(float NewValue, float MaxValue) const
{
    Healthbar->SetPercent(NewValue / MaxValue); // Assuming NewValue is a percentage
}

/**
 * Update the stamina value displayed on the HUD
 * @param NewValue The new stamina value to display
 * @param MaxValue The maximum stamina value to normalize against
*/
void UPGAS_InGame_HUD::UpdateStaminaValue(float NewValue, float MaxValue) const
{
    // Ensure the Staminabar is valid before updating
    if (Staminabar)
    {
        // Calculate the percentage and update the stamina bar
        // UE_LOG(LogTemp, Log, TEXT("Setting Stamina Bar Percent: %f"), NewValue / MaxValue);

        // Make sure the NewValue is not negative to avoid division by zero
        if (MaxValue <= 0.0f)
            return; // Avoid division by zero

        // Clamp the NewValue to ensure it doesn't exceed MaxValue
        NewValue = FMath::Clamp(NewValue, 0.0f, MaxValue);

        // Set the stamina bar percentage based on the new value and max value
        Staminabar->SetPercent(NewValue / MaxValue);
    }
}

/**
 * Update the adrenaline value displayed on the HUD
 * @param NewValue The new adrenaline value to display
 * @param MaxValue The maximum adrenaline value to normalize against
*/
void UPGAS_InGame_HUD::UpdateAdrenalineValue(float NewValue, float MaxValue) const
{
    Adrenalinebar->SetPercent(NewValue / MaxValue);
}

