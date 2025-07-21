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
 * Date: 7-10-2025
 * =============================================================================
 * PGAS_GE_InfiniteStaminaReduction
 * Gameplay Effect for reducing stamina infinitely (C++ version).
*/


#include "GAS/Effects/PGAS_GE_InfiniteStaminaReduction.h"
#include <GAS/AttributeSets/PlayerCharacterAttributeSet.h>

UPGAS_GE_InfiniteStaminaReduction::UPGAS_GE_InfiniteStaminaReduction()
{
    DurationPolicy = EGameplayEffectDurationType::Infinite;

    Period.Value = 0.35f; // How often the effect applies

    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UPlayerCharacterAttributeSet::GetStaminaAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;

    FSetByCallerFloat SetByCaller;
    SetByCaller.DataTag = StaminaReductionTag;
    ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

    Modifiers.Add(ModifierInfo);
}
