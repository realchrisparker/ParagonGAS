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
 * Enum defining Direction for character movement.
 */

#pragma once

#include "PGAS_Direction.generated.h"

UENUM(BlueprintType)
enum class EPGAS_Direction : uint8
{
    Forward  UMETA(DisplayName = "Forward"),
    Backward UMETA(DisplayName = "Backward"),
    Left     UMETA(DisplayName = "Left"),
    Right    UMETA(DisplayName = "Right")
};
