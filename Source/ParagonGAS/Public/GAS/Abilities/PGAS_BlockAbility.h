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
 * Date: 7-13-2025
 * =============================================================================
 * PGAS_BlockAbility
 * This ability enables the character to enter a blocking state, reducing or negating incoming damage.
 */

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"
#include "PGAS_BlockAbility.generated.h"

UCLASS(Blueprintable, meta= (DisplayName = "Block Ability"))
class PARAGONGAS_API UPGAS_BlockAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UPGAS_BlockAbility();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

    virtual void InputReleased(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
    /** Gameplay tag to indicate blocking state */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability",
        meta = (DisplayName = "Block Tag",
            ToolTip = "The gameplay tag that indicates the character is currently blocking. This is used to apply block effects and prevent damage. Also used as tag to activate the ability."))
    FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(FName("Character.Ability.Block"));

    /** Optional: Gameplay effect to apply while blocking (e.g., damage reduction) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability",
        meta = (DisplayName = "Stamina Effect Class",
            ToolTip = "The gameplay effect class to apply while blocking. This can be used to reduce damage taken while blocking."))
    TSubclassOf<class UGameplayEffect> StaminaReductionEffect = nullptr;

    /** The montage asset to play when this ability activates. Blueprint assignable. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability",
        meta = (DisplayName = "Montage To Play",
            ToolTip = "The animation montage to play when this ability is activated. This allows for the ability to play a specific animation montage when activated."))
    TObjectPtr<UAnimMontage> MontageToPlay;

private:
    /** Handle to the active gameplay effect (if any) */
    FActiveGameplayEffectHandle BlockEffectHandle;

    UFUNCTION()
    void OnMontageEnded();

    UFUNCTION()
    void PlayBlockMontage();
};
