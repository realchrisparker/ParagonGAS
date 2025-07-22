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
#include "PGAS_GE_MeleeDamageEffect.generated.h"

 /**
  * Melee damage effect—supports crits, knockback, and stun meta-data.
  */
UCLASS(BlueprintType, Blueprintable, Category = "Gameplay Ability System|Effects", meta = (DisplayName = "Melee Damage Gameplay Effect"))
class PARAGONGAS_API UPGAS_GE_MeleeDamageEffect : public UGameplayEffect
{
    GENERATED_BODY()

public:
    UPGAS_GE_MeleeDamageEffect();

    /** Type of damage (physical, fire, ice, etc.), as a gameplay tag. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    FGameplayTag DamageTypeTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Melee"));

    /** Amount of base damage this effect applies. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    float BaseDamage = 20.0f; // default value, can be overridden in instances
};
 