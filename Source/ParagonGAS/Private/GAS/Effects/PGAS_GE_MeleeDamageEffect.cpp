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
 * Date: 7-3-2025
 * =============================================================================
 * PGAS_GE_MeleeDamageEffect
 * Staff Melee Damage Gameplay Effect: Physical melee attack with staff, supporting crits, knockback, and stun.
 */

#include <GAS/Effects/PGAS_GE_MeleeDamageEffect.h>
#include <GAS/AttributeSets/EnemyAttributeSet.h>
#include <GAS/Effects/Calculations/PGAS_MeleeDamageExecutionCalc.h>

UPGAS_GE_MeleeDamageEffect::UPGAS_GE_MeleeDamageEffect()
{
    // All other inherited UGameplayEffect options (like DurationPolicy) can be set here as well.
    DurationPolicy = EGameplayEffectDurationType::Instant;

    // Remove direct Modifiers, execution handles all calculation
    Modifiers.Empty();

    // Setup execution calculation
    FGameplayEffectExecutionDefinition ExecDef;
    ExecDef.CalculationClass = UPGAS_MeleeDamageExecutionCalc::StaticClass();

    // If you want to capture attributes for use in the execution calculation, add them to ExecDef.Modifiers

    Executions.Add(ExecDef);
}
 