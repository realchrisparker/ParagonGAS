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
 * Date: 6-29-2025
 * =============================================================================
 * PGAS_ComboStep
 * An enumeration for the steps in a combo sequence.
 * This enum is used to track the current step in a combo ability, allowing for different actions
 */

#pragma once

#include "CoreMinimal.h"

/*
 * This enum is used to track the current step in a combo ability, allowing for different actions
*/
UENUM(BlueprintType)
enum class EPGASComboStep : uint8
{
    None    UMETA(DisplayName = "None"),
    Step1   UMETA(DisplayName = "Attack 1"),
    Step2   UMETA(DisplayName = "Attack 2"),
    Step3   UMETA(DisplayName = "Attack 3"),
    Step4   UMETA(DisplayName = "Attack 4"),
};