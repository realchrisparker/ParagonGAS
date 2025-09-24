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
 * Date: 8-19-2025
 * =============================================================================
 * EPGAS_WeaponHand
 * An enumeration for Which weapon/hand a window or action applies to.
 * This enum is used to determine the active weapon or hand for combat actions.
 */

#pragma once

#include "CoreMinimal.h"

/** Which weapon/hand a window or action applies to. */
UENUM(BlueprintType, meta=(DisplayName="Weapon Hand"))
enum class EPGAS_WeaponHand : uint8
{
    Left  UMETA(DisplayName = "Left"),
    Right UMETA(DisplayName = "Right"),
    Both  UMETA(DisplayName = "Both")
};

/** Which weapon/hand a window or action applies to. */
UENUM(BlueprintType, meta = (DisplayName = "Weapon Attack Type"))
enum class EPGAS_WeaponAttackType : uint8
{
    Light       UMETA(DisplayName = "Light"),
    Medium      UMETA(DisplayName = "Medium"),
    Heavy       UMETA(DisplayName = "Heavy"),
    JumpLight   UMETA(DisplayName = "Jump Light"),
    JumpMedium  UMETA(DisplayName = "Jump Medium"),
    JumpHeavy   UMETA(DisplayName = "Jump Heavy"),
    Land        UMETA(DisplayName = "Land"),
    Ranged      UMETA(DisplayName = "Ranged")
};