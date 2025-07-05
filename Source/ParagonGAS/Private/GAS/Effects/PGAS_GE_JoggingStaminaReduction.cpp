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
 * Date: 7-4-2025
 * =============================================================================
 * PGAS_GE_JoggingStaminaReduction
 * Gameplay Effect for reducing stamina while jogging (C++ version).
*/


#include "GAS/Effects/PGAS_GE_JoggingStaminaReduction.h"
#include <GAS/AttributeSets/PlayerCharacterAttributeSet.h>

UPGAS_GE_JoggingStaminaReduction::UPGAS_GE_JoggingStaminaReduction()
{
    // Set default values for the gameplay effect
    DurationPolicy = EGameplayEffectDurationType::Infinite;

    // Periodic application settings
    Period.Value = 0.5f;

    // Add a modifier to increase stamina over time
    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UPlayerCharacterAttributeSet::GetStaminaAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-2.0f));

    // Set the target tags for this modifier
    ModifierInfo.TargetTags.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Jogging")));
    ModifierInfo.TargetTags.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Alive")));

    // Ignore tags that should not trigger this effect
    ModifierInfo.TargetTags.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Combat")));

    Modifiers.Add(ModifierInfo);
}
