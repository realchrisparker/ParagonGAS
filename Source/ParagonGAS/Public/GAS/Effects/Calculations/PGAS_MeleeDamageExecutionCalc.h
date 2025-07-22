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
 * Date: 7-20-2025
 * =============================================================================
 * PGAS_MeleeDamageExecutionCalc.h
 * Basic melee damage execution calculation. Applies raw base damage from the effect.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PGAS_MeleeDamageExecutionCalc.generated.h"


/**
 * Melee Damage Execution Calculation
*/
UCLASS(meta = (DisplayName = "Melee Damage Execution Calculation"))
class PARAGONGAS_API UPGAS_MeleeDamageExecutionCalc : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()
    
public:
    UPGAS_MeleeDamageExecutionCalc();

    /**
     * Executes the melee damage calculation.
     * This will apply the base damage from the effect, optionally using SetByCaller data if available.
     * @param ExecutionParams Parameters for the execution, including the owning spec.
     * @param OutExecutionOutput Output modifiers to apply (e.g., damage).
    */
    virtual void Execute_Implementation(
        const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        FGameplayEffectCustomExecutionOutput& OutExecutionOutput
    ) const override;

protected:
    // Static definitions for captured attributes
    static FGameplayEffectAttributeCaptureDefinition AttackPowerDef;
    static FGameplayEffectAttributeCaptureDefinition DefenseDef;
};
