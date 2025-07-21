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

#include <GAS/Abilities/PGAS_BlockAbility.h>
#include <GAS/Effects/PGAS_GE_InfiniteStaminaReduction.h>
#include <Animations/PGAS_GameplayTagNotify.h>
#include <GAS/PGAS_AbilitySystemComponent.h>
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UPGAS_BlockAbility::UPGAS_BlockAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    bReplicateInputDirectly = true;

    // Ability Tag for matching Event
    SetAssetTags(FGameplayTagContainer(BlockTag));

    // Tags the owner actor ASC should have to activate ability
    ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove")));

    // Default Stamina Effect Class
    StaminaReductionEffect = UPGAS_GE_InfiniteStaminaReduction::StaticClass();

    // Setup montage to play when this ability activates
    static ConstructorHelpers::FObjectFinder<UAnimMontage> BlockMontageAsset(TEXT("/Game/ParagonSunWukong/Characters/Heroes/Wukong/Animations/Montages/AM_Block.AM_Block"));
    if (BlockMontageAsset.Succeeded())
    {
        MontageToPlay = BlockMontageAsset.Object;
    }
}

void UPGAS_BlockAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) // Handles cost, cooldown, etc.
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo(); // Get ASC from ActorInfo
    if (ASC)
    {
        // Add the block tag to the character
        ASC->AddLooseGameplayTag(BlockTag);
    }

    // If using input, keep ability active until input is released
    SetCanBeCanceled(true);
    SetShouldBlockOtherAbilities(false);

    if (StaminaReductionEffect && ActorInfo)
    {
        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(StaminaReductionEffect, GetAbilityLevel());
        SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Combat.Stamina.Reduction")), -0.090f); // Set the magnitude
        ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

    PlayBlockMontage();
}

void UPGAS_BlockAbility::InputReleased(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo)
{
    // End the block ability when input is released (if using hold-to-block)
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPGAS_BlockAbility::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (ASC)
    {
        // Remove the block tag and effect
        ASC->RemoveLooseGameplayTag(BlockTag);

        // Remove the active gameplay effect if it exists
        if (BlockEffectHandle.IsValid())
        {
            ASC->RemoveActiveGameplayEffect(BlockEffectHandle);
        }
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPGAS_BlockAbility::PlayBlockMontage()
{
    if (!MontageToPlay)
        return;

    UAbilityTask_PlayMontageAndWait* BlockMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this,
        NAME_None,
        MontageToPlay,
        1.0f,
        NAME_None,
        true
    );

    if (BlockMontageTask)
    {
        BlockMontageTask->OnCompleted.AddDynamic(this, &UPGAS_BlockAbility::OnMontageEnded);
        BlockMontageTask->OnInterrupted.AddDynamic(this, &UPGAS_BlockAbility::OnMontageEnded);
        BlockMontageTask->OnBlendOut.AddDynamic(this, &UPGAS_BlockAbility::OnMontageEnded);
        BlockMontageTask->ReadyForActivation();
    }
}

void UPGAS_BlockAbility::OnMontageEnded()
{
    if (IsActive() && MontageToPlay)
    {
        PlayBlockMontage();
    }
}