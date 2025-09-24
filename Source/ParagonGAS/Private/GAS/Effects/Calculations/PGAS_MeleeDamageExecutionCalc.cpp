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
 * Date: 7-20-2025
 * =============================================================================
 * PGAS_MeleeDamageExecutionCalc.cpp
 * Basic melee damage execution calculation. Applies raw base damage from the effect.
 */

#include <GAS/Effects/Calculations/PGAS_MeleeDamageExecutionCalc.h>
#include <GAS/Effects/PGAS_GE_MeleeDamageEffect.h>
#include <GAS/AttributeSets/EnemyAttributeSet.h>
#include <GAS/AttributeSets/PlayerCharacterAttributeSet.h>
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"

// Static definitions for captured attributes
FGameplayEffectAttributeCaptureDefinition UPGAS_MeleeDamageExecutionCalc::AttackPowerDef;
FGameplayEffectAttributeCaptureDefinition UPGAS_MeleeDamageExecutionCalc::DefenseDef;

// Constructor
UPGAS_MeleeDamageExecutionCalc::UPGAS_MeleeDamageExecutionCalc()
{    
    // Add the attributes we want to capture for this execution calculation
    AttackPowerDef = FGameplayEffectAttributeCaptureDefinition(
        UPlayerCharacterAttributeSet::GetAttackPowerAttribute(),
        EGameplayEffectAttributeCaptureSource::Source,
        true
    );
    DefenseDef = FGameplayEffectAttributeCaptureDefinition(
        UEnemyAttributeSet::GetDefenseAttribute(),
        EGameplayEffectAttributeCaptureSource::Target,
        true
    );
    
    RelevantAttributesToCapture.Add(AttackPowerDef);
    RelevantAttributesToCapture.Add(DefenseDef);
}

// Execute the melee damage calculation
// This will apply the base damage from the effect, optionally using SetByCaller data if available
void UPGAS_MeleeDamageExecutionCalc::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
    // Get Target ASC (the one receiving the effect)
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    if (!TargetASC)
    {
        return;
    }

    // Get the instigator ASC (the one that applied the effect)
    UAbilitySystemComponent* InstigatorASC = ExecutionParams.GetSourceAbilitySystemComponent();
    if (!InstigatorASC)
    {
        return;
    }

    ////=========================================
    //// Check if the target has the invincible state tag.
    ////=========================================
    if (TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Invincible"))))
    {
        // Apply zero damage as target is invincible
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(
                UEnemyAttributeSet::GetHealthAttribute(),  // Apply to health
                EGameplayModOp::Additive,                 // Additive (subtract from health)
                0.0f                                      // Zero damage
            )
        );
        return;
    }

    // Get the effect spec (contains SetByCaller data, magnitude, etc.)
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    // Get the base damage (default: 10.0f from your GE, but also support SetByCaller if used)
    float BaseDamage = 10.0f;

    // Try to pull SetByCaller if used (matches tag in your GE)
    static FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Melee"));

    ////=========================================
    //// Get Base Damage from SetByCaller or GE  
    ////=========================================
    // Get from SetByCaller if present
    float SetByCallerDamage = Spec.GetSetByCallerMagnitude(DamageTag, false, -1.f);
    if (SetByCallerDamage > 0.f)
    {
        BaseDamage = SetByCallerDamage;
    }
    else
    {
        // If not using SetByCaller, try grabbing the value from the GE if needed
        if (const UPGAS_GE_MeleeDamageEffect* MeleeGE = Cast<const UPGAS_GE_MeleeDamageEffect>(Spec.Def))
        {
            BaseDamage = MeleeGE->BaseDamage;
        }
    }

    ////=============================================
    //// Get Target defense and source attack power  
    ////=============================================

    FAggregatorEvaluateParameters EvalParams;
    EvalParams.SourceTags = &Spec.CapturedSourceTags.GetSpecTags();
    EvalParams.TargetTags = &Spec.CapturedTargetTags.GetSpecTags();

    float SourceAttackPower = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackPowerDef, EvalParams, SourceAttackPower);

    UE_LOG(LogTemp, Warning, TEXT("Melee Damage Execution: Source Attack Power = %f"), SourceAttackPower);

    float TargetDefense = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefenseDef, EvalParams, TargetDefense);

    if (TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Combat.Blocking"))))
    {
        // If the target is blocking, apply a defense multiplier (e.g., 0.5x)
        TargetDefense *= 2.f; // Adjust as needed for your game balance
        UE_LOG(LogTemp, Warning, TEXT("Melee Damage Execution: Target is blocking, Defense increased to %f"), TargetDefense);
    }

    UE_LOG(LogTemp, Warning, TEXT("Melee Damage Execution: Target Defense = %f"), TargetDefense);

    ////==========================================================================
    //// Calculate final damage                                                   
    //// This is a simple calculation: Damage = BaseDamage + AttackPower - Defense
    ////==========================================================================

    UE_LOG(LogTemp, Warning, TEXT("Melee Damage Execution: Base Damage = %f"), BaseDamage);

    float FinalDamage = BaseDamage + SourceAttackPower - TargetDefense;
    FinalDamage = FMath::Max(FinalDamage, 0.0f); // Ensure no negative damage

    UE_LOG(LogTemp, Warning, TEXT("Melee Damage Execution: Final Damage = %f"), FinalDamage);

    // Apply as negative to health (damage)
    OutExecutionOutput.AddOutputModifier(
        FGameplayModifierEvaluatedData(
            UEnemyAttributeSet::GetHealthAttribute(),  // Apply to health
            EGameplayModOp::Additive,                 // Additive (subtract from health)
            -FinalDamage                               // Negative = damage
        )
    );
}
