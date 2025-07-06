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
 * PGAS_GE_StaffMeleeDamageEffect
 * Staff Melee Damage Gameplay Effect: Physical melee attack with staff, supporting crits, knockback, and stun.
 */

#include "GAS/Effects/PGAS_GE_StaffMeleeDamageEffect.h"
#include <GAS/AttributeSets/EnemyAttributeSet.h>

UPGAS_GE_StaffMeleeDamageEffect::UPGAS_GE_StaffMeleeDamageEffect()
{
    // All other inherited UGameplayEffect options (like DurationPolicy) can be set here as well.
    DurationPolicy = EGameplayEffectDurationType::Instant;

    // Crit defaults
    bCanCrit = true;
    CritMultiplier = 2.0f; // Double damage on crit

    // Knockback & stun defaults
    KnockbackForce = 250.f; // Tweak as desired
    StunDuration = 0.5f;    // Half-second stun

    // Damage modifier for Health (SETUP THIS WAY)
    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UEnemyAttributeSet::GetHealthAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;

    FSetByCallerFloat SetByCaller;
    SetByCaller.DataTag = DamageTypeTag;
    ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

    Modifiers.Add(ModifierInfo);
}

float UPGAS_GE_StaffMeleeDamageEffect::CalculateFinalDamage_Implementation(bool bIsCritical) const
{
    if (bIsCritical && bCanCrit)
    {
        return BaseDamage * CritMultiplier;
    }
    return BaseDamage;
}
 