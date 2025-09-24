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
 * PGAS_GE_RangedDamageEffect.h
 * Defines a GameplayEffect for ranged damage (bows/arrows).
 * Uses an execution calculation class to apply damage using SetByCaller values.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include <GAS/AttributeSets/PlayerCharacterAttributeSet.h>
#include <GAS/AttributeSets/EnemyAttributeSet.h>
#include <GAS/Effects/Calculations/PGAS_RangedDamageExecutionCalc.h>
#include "PGAS_GE_RangedDamageEffect.generated.h"


/**
 * Ranged damage GameplayEffect — applies damage via execution calculation.
 * Supports SetByCaller magnitudes (e.g., bow charge, crit multipliers).
 */
UCLASS(BlueprintType, Blueprintable, Category = "Gameplay Ability System|Effects", meta = (DisplayName = "Ranged Damage Gameplay Effect"))
class PARAGONGAS_API UPGAS_GE_RangedDamageEffect : public UGameplayEffect
{
    GENERATED_BODY()

public:
    // Constructor
    UPGAS_GE_RangedDamageEffect()
    {
        DurationPolicy = EGameplayEffectDurationType::Instant;

        // No direct modifiers — execution handles calculation
        Modifiers.Empty();

        // Set up execution calculation
        FGameplayEffectExecutionDefinition ExecDef;
        ExecDef.CalculationClass = UPGAS_RangedDamageExecutionCalc::StaticClass();
        Executions.Add(ExecDef);
    }

    /** Type of damage (default: ranged physical) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    FGameplayTag DamageTypeTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Ranged"));

    /** Default base damage (can be overridden by SetByCaller or ability logic) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    float BaseDamage = 10.0f;
};
