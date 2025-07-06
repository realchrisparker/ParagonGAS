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
 * PGAS_GE_AttackStaminaReduction
 * Gameplay Effect for reducing stamina on attack (C++ version).
*/


#include "GAS/Effects/PGAS_GE_AttackStaminaReduction.h"
#include <GAS/AttributeSets/PlayerCharacterAttributeSet.h>


UPGAS_GE_AttackStaminaReduction::UPGAS_GE_AttackStaminaReduction()
{
    // Set default values for the gameplay effect
    DurationPolicy = EGameplayEffectDurationType::Instant;

    // Add a modifier to increase stamina over time
    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UPlayerCharacterAttributeSet::GetStaminaAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-5.0f)); // Reduce stamina by 5 on attack

    Modifiers.Add(ModifierInfo);
}
