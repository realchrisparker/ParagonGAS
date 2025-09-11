/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * Author: Christopher D. Parker
 * Date: 9-6-2025
 * =============================================================================
 * Defines hit reaction enums for data-driven hit reactions.
 */

#pragma once

#include "CoreMinimal.h"


/** Enum for hit reaction direction */
UENUM(BlueprintType, meta=(DisplayName = "Hit Direction"))
enum class EPGAS_HitDirection : uint8
{
    None      UMETA(DisplayName = "None"),
    Forward   UMETA(DisplayName = "Forward"),
    Back      UMETA(DisplayName = "Back"),
    Left      UMETA(DisplayName = "Left"),
    Right     UMETA(DisplayName = "Right"),
};

/** Enum for hit reaction severity */
UENUM(BlueprintType, meta=(DisplayName = "Hit Severity"))
enum class EPGAS_HitSeverity : uint8
{
    Light       UMETA(DisplayName = "Light (Quick Flinch)"),
    Heavy       UMETA(DisplayName = "Heavy (Strong Hit)"),
    Stagger     UMETA(DisplayName = "Stagger (Medium Interrupt)"),
    Knockback   UMETA(DisplayName = "Knockback (Pushed Back)"),
    Knockdown   UMETA(DisplayName = "Knockdown (Fall to Ground)"),
    Dazed       UMETA(DisplayName = "Dazed (Stunned / Groggy)"),
    Critical    UMETA(DisplayName = "Critical (Launch / Fatal)"),
    Death       UMETA(DisplayName = "Death (Die / Ragdoll)"),
};
