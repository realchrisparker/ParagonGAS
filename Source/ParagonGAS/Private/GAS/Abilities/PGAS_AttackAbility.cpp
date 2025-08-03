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
 * Date: 7-30-2025
 * =============================================================================
 * PGAS_AttackAbility.cpp
 * Implementation of the basic attack ability.
 */

#include "GAS/Abilities/PGAS_AttackAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include <Animations/PGAS_GameplayTagNotify.h>
#include <GAS/PGAS_AbilitySystemComponent.h>
#include <Characters/Base/PGAS_CharacterBase.h>


UPGAS_AttackAbility::UPGAS_AttackAbility()
{
    // Make one instance per actor (customize as needed)
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // This is the tag by which you will activate the ability:
    SetAssetTags(FGameplayTagContainer(ActivationTag));

    // Tags the owner actor ASC should have to activate ability
    ActivationOwnedTags = OwnedActivationTags;
}

void UPGAS_AttackAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    // Cache the parameters for later use
    CachedHandle = Handle;
    CachedActorInfo = ActorInfo;
    CachedActivationInfo = ActivationInfo;
    CachedTriggerEventData = TriggerEventData;

    // Commit cost/cooldown; if it fails, cancel immediately
    if (!MontageToPlay || !CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        // If we can't commit the ability, end it immediately
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    
    if (MontageToPlay && ActorInfo && ActorInfo->AvatarActor.IsValid())
    {
        UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
        if (ASC)
        {
            // // Loop through the montage notifies and setup notifications we can trigger events from.
            // for (const FAnimNotifyEvent& Notify : MontageToPlay->Notifies)
            // {
            //     UPGAS_GameplayTagNotify* GameplayTagNotify = Cast<UPGAS_GameplayTagNotify>(Notify.NotifyStateClass);
            //     if (GameplayTagNotify)
            //     {
            //         // Subscribe to the notify begin and end events by the GameplayTagNotify notify begin and end gameplay tags.
            //         UAbilityTask_WaitGameplayEvent* WaitEventBegin = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GameplayTagNotify->BeginNotifyTag);
            //         if (WaitEventBegin)
            //         {
            //             WaitEventBegin->EventReceived.AddDynamic(this, &UPGAS_GameplayAbility_Montage::OnNotifyBegin);
            //             WaitEventBegin->ReadyForActivation();
            //         }

            //         UAbilityTask_WaitGameplayEvent* WaitEventEnd = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GameplayTagNotify->EndNotifyTag);
            //         if (WaitEventEnd)
            //         {
            //             WaitEventEnd->EventReceived.AddDynamic(this, &UPGAS_GameplayAbility_Montage::OnNotifyEnd);
            //             WaitEventEnd->ReadyForActivation();
            //         }
            //     }
            //     else
            //     {
            //         // Handle non-gameplay tag notifies (Regular notifiers)
            //         MontageNotify(Notify.NotifyName); // Call the Blueprint event for the notify
            //     }
            // }

            // Create and activate the montage task (this immediately plays the montage)
            UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
                this,
                NAME_None,      // Optional task instance name
                MontageToPlay,
                1.0f,           // PlayRate
                NAME_None,      // StartSection
                false           // bStopWhenAbilityEnds
            );

            if (MontageTask)
            {
                // MontageTask->OnCompleted.AddDynamic(this, &UPGAS_GameplayAbility_Montage::OnMontageCompleted);
                // MontageTask->OnInterrupted.AddDynamic(this, &UPGAS_GameplayAbility_Montage::OnMontageInterrupted);
                // MontageTask->OnCancelled.AddDynamic(this, &UPGAS_GameplayAbility_Montage::OnMontageCancelled);
                MontageTask->ReadyForActivation(); // Start the task!
            }
        }
    }
}

void UPGAS_AttackAbility::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
