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
 * Date: 8-17-2025
 * =============================================================================
 * Attack type struct
 */

#pragma once

#include "CoreMinimal.h"
#include "PGAS_AttackHitbox.generated.h"

USTRUCT(BlueprintType, Blueprintable, meta = (DisplayName = "PGAS Attack Hitbox", Description = "Represents an attack hitbox in the PGAS Combat System"))
struct PARAGONGAS_API FPGAS_AttackHitbox
{
    GENERATED_BODY()

public:

    /** Start socket (e.g., "weapon_start"). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    FName StartSocket = NAME_None;

    /** End socket (e.g., "weapon_end"). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    FName EndSocket = NAME_None;

    /** Sweep radius. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox", meta = (ClampMin = "0.0"))
    float Radius = 10.f;

    /** Debug draw toggle for this attack. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    bool bDebugDraw = true;
};