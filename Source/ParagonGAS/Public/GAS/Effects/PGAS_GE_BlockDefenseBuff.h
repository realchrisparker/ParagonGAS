/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * Author: Christopher D. Parker
 * Date: 8-31-2025
 * =============================================================================
 * PGAS_GE_BlockDefenseBuff.h
 * Gameplay Effect that increases Defense while blocking.
 * Uses SetByCaller magnitude so designers can scale block defense dynamically.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GAS/AttributeSets/PlayerCharacterAttributeSet.h"
#include "GAS/AttributeSets/EnemyAttributeSet.h"
#include "PGAS_GE_BlockDefenseBuff.generated.h"

UCLASS(BlueprintType, Blueprintable, Category = "Gameplay Ability System|Effects", meta = (DisplayName = "Block Defense Buff"))
class PARAGONGAS_API UPGAS_GE_BlockDefenseBuff : public UGameplayEffect
{
    GENERATED_BODY()

public:
    UPGAS_GE_BlockDefenseBuff()
    {
        DurationPolicy = EGameplayEffectDurationType::Infinite; // persists until manually removed

        // Common tag used to pass in block defense value
        FGameplayTag BlockDefenseTag = FGameplayTag::RequestGameplayTag(FName("Combat.Blocking"));

        // --- Player Defense modifier ---
        {
            FGameplayModifierInfo PlayerMod;
            PlayerMod.Attribute = UPlayerCharacterAttributeSet::GetDefenseAttribute();
            PlayerMod.ModifierOp = EGameplayModOp::Additive;

            FSetByCallerFloat SetByCaller;
            SetByCaller.DataTag = BlockDefenseTag;
            PlayerMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

            Modifiers.Add(PlayerMod);
        }

        // --- Enemy Defense modifier ---
        {
            FGameplayModifierInfo EnemyMod;
            EnemyMod.Attribute = UEnemyAttributeSet::GetDefenseAttribute();
            EnemyMod.ModifierOp = EGameplayModOp::Additive;

            FSetByCallerFloat SetByCaller;
            SetByCaller.DataTag = BlockDefenseTag;
            EnemyMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

            Modifiers.Add(EnemyMod);
        }
    }
};
