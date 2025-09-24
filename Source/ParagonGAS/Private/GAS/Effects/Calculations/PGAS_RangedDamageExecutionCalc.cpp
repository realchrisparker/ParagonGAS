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
 * PGAS_RangedDamageExecutionCalc.cpp
 * Basic ranged damage execution calculation. Applies base damage, supports SetByCaller,
 * and factors in source AttackPower and target Defense, just like melee.
 */

#include "GAS/Effects/Calculations/PGAS_RangedDamageExecutionCalc.h"
#include "GAS/Effects/PGAS_GE_RangedDamageEffect.h"
#include "GAS/AttributeSets/EnemyAttributeSet.h"
#include "GAS/AttributeSets/PlayerCharacterAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

 // Static definitions for captured attributes
FGameplayEffectAttributeCaptureDefinition UPGAS_RangedDamageExecutionCalc::AttackPowerDef;
FGameplayEffectAttributeCaptureDefinition UPGAS_RangedDamageExecutionCalc::DefenseDef;

UPGAS_RangedDamageExecutionCalc::UPGAS_RangedDamageExecutionCalc()
{
    // Capture AttackPower from source (archer / player character)
    AttackPowerDef = FGameplayEffectAttributeCaptureDefinition(
        UPlayerCharacterAttributeSet::GetAttackPowerAttribute(),
        EGameplayEffectAttributeCaptureSource::Source,
        true
    );

    // Capture Defense from target (enemy / victim)
    DefenseDef = FGameplayEffectAttributeCaptureDefinition(
        UEnemyAttributeSet::GetDefenseAttribute(),
        EGameplayEffectAttributeCaptureSource::Target,
        true
    );

    RelevantAttributesToCapture.Add(AttackPowerDef);
    RelevantAttributesToCapture.Add(DefenseDef);
}

void UPGAS_RangedDamageExecutionCalc::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    if (!TargetASC) return;

    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    if (!SourceASC) return;

    // Invincibility check
    if (TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Invincible"))))
    {
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
            UEnemyAttributeSet::GetHealthAttribute(),
            EGameplayModOp::Additive,
            0.0f
        ));
        return;
    }

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    ////=======================================
    //// Base Damage (via SetByCaller or GE)
    ////=======================================
    float BaseDamage = 10.0f;
    static FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Ranged"));

    float SetByCallerDamage = Spec.GetSetByCallerMagnitude(DamageTag, false, -1.f);
    if (SetByCallerDamage > 0.f)
    {
        BaseDamage = SetByCallerDamage;
    }
    else if (const UPGAS_GE_RangedDamageEffect* RangedGE = Cast<const UPGAS_GE_RangedDamageEffect>(Spec.Def))
    {
        BaseDamage = RangedGE->BaseDamage;
    }

    ////=======================================
    //// AttackPower / Defense
    ////=======================================
    FAggregatorEvaluateParameters EvalParams;
    EvalParams.SourceTags = &Spec.CapturedSourceTags.GetSpecTags();
    EvalParams.TargetTags = &Spec.CapturedTargetTags.GetSpecTags();

    float SourceAttackPower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackPowerDef, EvalParams, SourceAttackPower);

    float TargetDefense = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefenseDef, EvalParams, TargetDefense);

    if (TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Combat.Blocking"))))
    {
        TargetDefense *= 2.f; // arrows do less damage if shield is up
    }

    ////=======================================
    //// Final Damage Formula
    ////=======================================
    float FinalDamage = BaseDamage + SourceAttackPower - TargetDefense;
    FinalDamage = FMath::Max(FinalDamage, 0.0f);

    // Apply negative value to Health (damage)
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
        UEnemyAttributeSet::GetHealthAttribute(),
        EGameplayModOp::Additive,
        -FinalDamage
    ));
}
