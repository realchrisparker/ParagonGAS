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
 * Date: 7-28-2025
 * =============================================================================
 * Enum defining the types of stimulus senses used in FPGAS_ST_StimulusSense.
 */

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, meta = (DisplayName = "Stimulus Sense"))
enum class EPGAS_StimulusSenseType : uint8
{
    Unknown UMETA(DisplayName = "Unknown"),
    Sight   UMETA(DisplayName = "Sight"),
    Hearing UMETA(DisplayName = "Hearing"),
    Damage  UMETA(DisplayName = "Damage")
};