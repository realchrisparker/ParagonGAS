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
 * PGAS_GA_Jump
 * Simple Gameplay Ability to trigger character jump using GAS.
 */

#include "GAS/Abilities/PGAS_JumpAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Characters/Base/PGAS_CharacterBase.h"

UPGAS_JumpAbility::UPGAS_JumpAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    bReplicateInputDirectly = true;

    // Ability Tag for matching Event
    SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Character.Ability.Jump"))));

    ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove")));
}

void UPGAS_JumpAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    UE_LOG(LogTemp, Warning, TEXT("UPGAS_JumpAbility::ActivateAbility called for %s"), *GetNameSafe(ActorInfo->AvatarActor.Get()));

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) // Handles cost, cooldown, etc.
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (APGAS_CharacterBase* Character = Cast<APGAS_CharacterBase>(ActorInfo->AvatarActor.Get()))
    {
        Character->Jump();
        // You can trigger VFX/SFX here
    }
}

void UPGAS_JumpAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo)
{
    Super::InputReleased(Handle, ActorInfo, ActivationInfo);

    if (APGAS_CharacterBase* Character = Cast<APGAS_CharacterBase>(ActorInfo->AvatarActor.Get()))
    {
        Character->StopJumping();
    }
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPGAS_JumpAbility::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    UE_LOG(LogTemp, Warning, TEXT("UPGAS_JumpAbility::EndAbility called for %s"), *GetNameSafe(ActorInfo->AvatarActor.Get()));

    if (APGAS_CharacterBase* Character = Cast<APGAS_CharacterBase>(ActorInfo->AvatarActor.Get()))
    {
        Character->StopJumping();
    }
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
