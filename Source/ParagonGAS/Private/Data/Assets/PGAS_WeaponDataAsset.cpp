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
 * Date: 8-24-2025
 * =============================================================================
 * Defines a data-driven schema for weapon configurations, enabling dual-wielding,
 * hitbox profiles, attack montages, gameplay effects, and GAS tags.
 */

#include "Data/Assets/PGAS_WeaponDataAsset.h"

float UPGAS_WeaponDataAsset::GetAttackDamage(bool bIsLeftHand, bool bIsHeavyAttack) const
{
    const FPGAS_WeaponHandProfile& Profile = bIsLeftHand ? LeftHandProfile : RightHandProfile;
    return bIsHeavyAttack ? Profile.HeavyAttackDamage : Profile.LightAttackDamage;
}

float UPGAS_WeaponDataAsset::GetAttackStaminaCost(bool bIsLeftHand, bool bIsHeavyAttack) const
{
    const FPGAS_WeaponHandProfile& Profile = bIsLeftHand ? LeftHandProfile : RightHandProfile;
    return bIsHeavyAttack ? Profile.HeavyAttackStaminaCost : Profile.LightAttackStaminaCost;
}

FPGAS_AttackData UPGAS_WeaponDataAsset::GetAttackData(bool bIsLeftHand, bool bIsHeavyAttack) const
{
    const FPGAS_WeaponHandProfile& Profile = bIsLeftHand ? LeftHandProfile : RightHandProfile;

    FPGAS_AttackData Data;
    Data.Damage = bIsHeavyAttack ? Profile.HeavyAttackDamage : Profile.LightAttackDamage;
    Data.StaminaCost = bIsHeavyAttack ? Profile.HeavyAttackStaminaCost : Profile.LightAttackStaminaCost;
    Data.AttackMontage = bIsHeavyAttack ? Profile.HeavyAttackMontage : Profile.LightAttackMontage;
    Data.OnHitEffects = Profile.OnHitEffects;
    Data.AbilityTag = bIsHeavyAttack ? Profile.HeavyAttackAbilityTag : Profile.LightAttackAbilityTag;

    return Data;
}
