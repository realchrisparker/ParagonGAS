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
 * Date: 8-20-2025
 * =============================================================================
 * Implementation of the base Montage Gameplay Ability class.
 */

#include "GAS/Abilities/PGAS_GameplayAbility_MontageBase.h"
#include "Characters/Base/PGAS_CharacterBase.h"
#include "GAS/PGAS_AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

 /** Constructor */
UPGAS_GameplayAbility_MontageBase::UPGAS_GameplayAbility_MontageBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

/**
 * Called when the ability is assigned to an avatar.
 * We use it to cache references to the character and its components.
 */
void UPGAS_GameplayAbility_MontageBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);

    // Cache references for easy access in abilities
    if (ActorInfo)
    {
        CachedCharacter = Cast<APGAS_CharacterBase>(ActorInfo->AvatarActor.Get());
        CachedASC = Cast<UPGAS_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

        if (CachedCharacter)
        {
            CachedAnimInstance = CachedCharacter->GetMesh() ? CachedCharacter->GetMesh()->GetAnimInstance() : nullptr;
            CachedCharacterLevel = CachedCharacter->GetCharacterLevel();
        }
    }
}

/**
 * Activate the ability
 * This function is called to activate the gameplay ability.
 * @param Handle The handle of the ability spec.
 * @param ActorInfo The actor info for the ability.
 * @param ActivationInfo The activation info for the ability.
 * @param TriggerEventData The trigger event data for the ability.
 */
void UPGAS_GameplayAbility_MontageBase::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) // pay costs/cooldowns
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // Call into BP
    OnAbilityActivatedBlueprint();

    // NOTE: CreatePlayMontageAndWaitProxy uses ASC → AvatarActor → Mesh → AnimInstance under the hood with GAS, so CachedAnimInstance is a safety check only.
    // We don't actually do anything else with it. Since it's just a pointer to memory we aren't losing any performance doing this check.
    if (MontageToPlay && CachedAnimInstance)
    {
        UAbilityTask_PlayMontageAndWait* MontageTask =
            UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay, 1.f, NAME_None, false, 1.f);

        if (MontageTask)
        {
            MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
            MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageCancelled);
            MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCancelled);

            MontageTask->ReadyForActivation();
        }
    }
    else
    {
        // No montage, just end immediately
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }
}

/**
 * Called when the ability ends.
 * @param Handle The handle of the ability spec.
 * @param ActorInfo The actor info for the ability.
 * @param ActivationInfo The activation info for the ability.
 * @param bReplicateEndAbility Whether to replicate the end ability to other clients.
 * @param bWasCancelled Whether the ability was cancelled.
 */
void UPGAS_GameplayAbility_MontageBase::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    // Call into BP
    OnAbilityEndedBlueprint();
}

void UPGAS_GameplayAbility_MontageBase::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPGAS_GameplayAbility_MontageBase::OnMontageCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}