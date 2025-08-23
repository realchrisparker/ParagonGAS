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
 * PGAS_GE_InstantStaminaReduction
 * Gameplay Effect for reducing stamina instantly (C++ version).
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include <GAS/AttributeSets/PlayerCharacterAttributeSet.h>
#include "PGAS_GE_InstantStaminaReduction.generated.h"

UCLASS(BlueprintType, Blueprintable, Category = "Gameplay Ability System|Effects", meta = (DisplayName = "Instant Stamina Reduction"))
class PARAGONGAS_API UPGAS_GE_InstantStaminaReduction : public UGameplayEffect
{
    GENERATED_BODY()

public:
    // Constructor
    UPGAS_GE_InstantStaminaReduction()
    {
        DurationPolicy = EGameplayEffectDurationType::Instant;

        Period.Value = 0.35f; // How often the effect applies

        FGameplayModifierInfo ModifierInfo;
        ModifierInfo.Attribute = UPlayerCharacterAttributeSet::GetStaminaAttribute();
        ModifierInfo.ModifierOp = EGameplayModOp::Additive;

        FSetByCallerFloat SetByCaller;
        SetByCaller.DataTag = StaminaReductionTag;
        ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

        Modifiers.Add(ModifierInfo);
    }

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect|Config")
    FGameplayTag StaminaReductionTag = FGameplayTag::RequestGameplayTag(FName("Combat.Stamina.Reduction"));
};