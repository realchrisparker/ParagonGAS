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
 * Date: 9-20-2025
 * =============================================================================
 * PGAS_RangedDamageExecutionCalc.h
 * Basic ranged damage execution calculation. Supports SetByCaller magnitudes.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PGAS_RangedDamageExecutionCalc.generated.h"

 /**
  * Ranged Damage Execution Calculation
  * - Pulls damage magnitude from SetByCaller (e.g., Combat.Damage.Event.Ranged).
  * - Optionally uses attributes like AttackPower / Defense.
  */
UCLASS(meta = (DisplayName = "Ranged Damage Execution Calculation"))
class PARAGONGAS_API UPGAS_RangedDamageExecutionCalc : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    UPGAS_RangedDamageExecutionCalc();

    virtual void Execute_Implementation(
        const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        FGameplayEffectCustomExecutionOutput& OutExecutionOutput
    ) const override;

protected:
    // Static definitions for captured attributes (optional for scaling)
    static FGameplayEffectAttributeCaptureDefinition AttackPowerDef;
    static FGameplayEffectAttributeCaptureDefinition DefenseDef;
};
