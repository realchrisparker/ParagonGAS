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
 * Struct to define movement speed values for different movement states like Sneak, Walk, Escalated, and Attack.
 */

#pragma once

#include "CoreMinimal.h"
#include "PGAS_ST_MovementSpeed.generated.h"

USTRUCT(BlueprintType, meta=(DisplayName = "Movement Speed Struct"))
struct PARAGONGAS_API FPGAS_ST_MovementSpeed
{
    GENERATED_BODY()

public:
    /** Speed value for Sneak movement */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Sneak = 150.0f;

    /** Speed value for Walk movement */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Walk = 300.0f;

    /** Speed value for Escalated movement */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Escalated = 450.0f;

    /** Speed value for Attack movement */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Attack = 200.0f;
};
