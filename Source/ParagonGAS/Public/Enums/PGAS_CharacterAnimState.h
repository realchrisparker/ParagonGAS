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
 * Date: 10-10-2025
 * =============================================================================
 * Enum defining character-level animation states for use in animation logic,
 * pose search chooser tables, and gameplay-driven state selection.
 */

#pragma once

#include "PGAS_CharacterAnimState.generated.h"

 /** High-level character animation states for Motion Matching and Chooser Tables */
UENUM(BlueprintType)
enum class EPGAS_CharacterAnimState : uint8
{
    // Idle states
    StandingIdle        UMETA(DisplayName = "Standing Idle"),
    CrouchedIdle        UMETA(DisplayName = "Crouched Idle"),

    // In-air state
    InAir               UMETA(DisplayName = "In Air"),

    // Movement states
    Walking             UMETA(DisplayName = "Walking"),
    CrouchedWalking     UMETA(DisplayName = "Crouched Walking"),
    Jogging             UMETA(DisplayName = "Jogging"),
    Running             UMETA(DisplayName = "Running"),

    // Blocking states
    BlockingIdle        UMETA(DisplayName = "Blocking Idle"),
    BlockingWalking     UMETA(DisplayName = "Blocking Walking"),
    BlockingJogging     UMETA(DisplayName = "Blocking Jogging"),
    BlockingRunning     UMETA(DisplayName = "Blocking Running")
};
