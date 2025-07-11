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
 * Date: 7-9-2025
 * =============================================================================
 * PGAS_JumpAbility
 * Simple Gameplay Ability to trigger character jump using GAS.
 */

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Sound/SoundCue.h"
#include "PGAS_JumpAbility.generated.h"

UCLASS(Blueprintable, meta=(DisplayName="Jump Ability"))
class PARAGONGAS_API UPGAS_JumpAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UPGAS_JumpAbility();

    // Called when the ability is activated
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual void InputReleased(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo) override;

    // Called when the ability ends or is cancelled
    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

protected:
    // Optionally, add a GameplayTag to restrict when you can jump (e.g. "State.CanJump")
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    FGameplayTag RequiredTag;

    // Optionally, add a sound cue to play when jumping
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Audio")
    TObjectPtr<USoundBase> JumpSoundCue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Cost")
    TSubclassOf<class UGameplayEffect> StaminaReductionEffect;
};
 