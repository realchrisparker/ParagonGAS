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

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "PGAS_GE_StaffMeleeDamageEffect.generated.h"

 /**
  * Staff melee damage effect—supports crits, knockback, and stun meta-data.
  */
UCLASS(BlueprintType, Blueprintable, Category = "Gameplay Ability System|Effects", meta = (DisplayName = "Staff Melee Damage Gameplay Effect"))
class PARAGONGAS_API UPGAS_GE_StaffMeleeDamageEffect : public UGameplayEffect
{
    GENERATED_BODY()

public:
    UPGAS_GE_StaffMeleeDamageEffect();

    /** Type of damage (physical, fire, ice, etc.), as a gameplay tag. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    FGameplayTag DamageTypeTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Melee.Staff"));

    /** Amount of base damage this effect applies. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    float BaseDamage = 20.0f; // default value, can be overridden in instances

    /** Whether this staff attack can crit. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Critical")
    bool bCanCrit;

    /** Crit damage multiplier (applied if a critical hit). */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Critical", meta = (EditCondition = "bCanCrit", ClampMin = "1.0"))
    float CritMultiplier;

    /** Optional: Knockback force to apply with this hit (0 = no knockback). */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Meta")
    float KnockbackForce;

    /** Optional: Duration (in seconds) of stun this effect applies (0 = no stun). */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Meta")
    float StunDuration;

    /** Calculates final damage (can be overridden in Blueprint or C++). */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    float CalculateFinalDamage(bool bIsCritical) const;

    /** Returns true if this effect should apply a stun. */
    UFUNCTION(BlueprintPure, Category = "Damage")
    bool HasStun() const { return StunDuration > 0.f; }

    /** Returns true if this effect should apply knockback. */
    UFUNCTION(BlueprintPure, Category = "Damage")
    bool HasKnockback() const { return KnockbackForce > 0.f; }
};
 