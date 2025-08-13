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
    Unknown    UMETA(DisplayName = "Unknown"),
    Idle       UMETA(DisplayName = "Idle"),
    Patrol     UMETA(DisplayName = "Patrol"),
    Attack     UMETA(DisplayName = "Attack")
};