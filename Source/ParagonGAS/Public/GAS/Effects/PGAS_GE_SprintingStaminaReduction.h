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
 * UPGAS_GE_SprintingStaminaReduction
 * Gameplay Effect for reducing stamina while jogging (C++ version).
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include <GAS/AttributeSets/PlayerCharacterAttributeSet.h>
#include "PGAS_GE_SprintingStaminaReduction.generated.h"

UCLASS(BlueprintType, Blueprintable, Category = "Gameplay Ability System|Effects", meta = (DisplayName = "Sprinting Stamina Reduction"))
class PARAGONGAS_API UPGAS_GE_SprintingStaminaReduction : public UGameplayEffect
{
    GENERATED_BODY()

public:
    // Constructor
    UPGAS_GE_SprintingStaminaReduction() {
        // Set default values for the gameplay effect
        DurationPolicy = EGameplayEffectDurationType::Infinite;

        // Periodic application settings
        Period.Value = 0.35f;

        // Add a modifier to increase stamina over time
        FGameplayModifierInfo ModifierInfo;
        ModifierInfo.Attribute = UPlayerCharacterAttributeSet::GetStaminaAttribute();
        ModifierInfo.ModifierOp = EGameplayModOp::Additive;
        ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-2.75f));

        // Set the target tags for this modifier
        // ModifierInfo.TargetTags.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Sprinting")));
        ModifierInfo.TargetTags.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Alive")));

        Modifiers.Add(ModifierInfo);
    }
};