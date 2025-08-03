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
 * Date: 7-29-2025
 * =============================================================================
 * Enum defining the events that can be sent to the StateTree.
 */

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, meta = (DisplayName = "State Tree Event"))
enum class EPGAS_StateTreeEvent : uint8
{
    Idle    UMETA(DisplayName = "Idle"),
    Attack  UMETA(DisplayName = "Attack"),
    Patrol  UMETA(DisplayName = "Patrol"),
    Flee    UMETA(DisplayName = "Flee"),
    Search  UMETA(DisplayName = "Search"),
};