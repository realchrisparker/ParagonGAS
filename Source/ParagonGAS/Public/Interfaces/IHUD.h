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
 * Date: 7-1-2025
 * =============================================================================
 * IHUD Interface
 * Blueprint-pure interface for updating HUD elements like the health bar.
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHUD.generated.h"

UINTERFACE(BlueprintType, meta = (DisplayName = "HUD Interface"))
class PARAGONGAS_API UIHUD : public UInterface
{
    GENERATED_BODY()
};

class PARAGONGAS_API IIHUD
{
    GENERATED_BODY()

public:
    // Blueprint pure, so no implementation required here.

    /**
     * Update the health bar UI with the given percentage.
     * @param NewPercentage - The new percentage to update the health bar.
     * @return The new percentage after update (may be clamped or processed).
     */
    UFUNCTION(Category = "HUD", meta = (DisplayName = "Update Health Bar"))
    virtual float UpdateHealthBar(float NewValue, float MaxValue) const = 0;

    /**
     * Update the mana bar UI with the given percentage.
     * @param NewPercentage - The new percentage to update the mana bar.
     * @return The new percentage after update.
     */
    UFUNCTION(Category = "HUD", meta = (DisplayName = "Update Adrenaline Bar"))
    virtual float UpdateAdrenalineBar(float NewValue, float MaxValue) const = 0;

    /**
     * Update the stamina bar UI with the given percentage.
     * @param NewPercentage - The new percentage to update the stamina bar.
     * @return The new percentage after update.
     */
    UFUNCTION(Category = "HUD", meta = (DisplayName = "Update Stamina Bar"))
    virtual float UpdateStaminaBar(float NewValue, float MaxValue) const = 0;

    /**
     * Update the experience bar UI with the given percentage.
     * @param NewPercentage - The new percentage to update the experience bar.
     * @return The new percentage after update.
     */
    UFUNCTION(Category = "HUD", meta = (DisplayName = "Update Experience Bar"))
    virtual float UpdateExperienceBar(float NewValue, float MaxValue) const = 0;
};
 