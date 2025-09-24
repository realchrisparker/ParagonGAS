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
 * Date: 8-21-2025
 * =============================================================================
 * Enum defining Input IDs for abilities that use GAS input binding.
 */

#pragma once

#include "PGAS_AbilityInputID.generated.h"

UENUM(BlueprintType)
enum class EPGAS_AbilityInputID : uint8
{
    None       UMETA(DisplayName = "None"),
    Sprint     UMETA(DisplayName = "Sprint"),
    Block      UMETA(DisplayName = "Block"),
    BowAttack  UMETA(DisplayName = "Bow Attack"),
};
