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
 * Date: 7-27-2025
 * =============================================================================
 * Enum defining the movement speed categories used in FPGAS_ST_MovementSpeed.
 */

#pragma once

#include "CoreMinimal.h"
#include "PGAS_MovementSpeedType.generated.h"

UENUM(BlueprintType, meta= (DisplayName = "Movement Speed Type"))
enum class EPGAS_MovementSpeedType : uint8
{
    Sneak     UMETA(DisplayName = "Sneak"),
    Walk      UMETA(DisplayName = "Walk"),
    Escalated UMETA(DisplayName = "Escalated"),
    Attack    UMETA(DisplayName = "Attack")
};
