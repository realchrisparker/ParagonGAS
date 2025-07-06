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
 * Date: 6-29-2025
 * =============================================================================
 * PGAS_GameplayAbility_Montage Implementation
 */

#include "GAS/Abilities/PGAS_GameplayAbility_Montage.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include <Animations/PGAS_GameplayTagNotify.h>
#include <GAS/PGAS_AbilitySystemComponent.h>
#include <Characters/Base/PGAS_CharacterBase.h>


 // Constructor
UPGAS_GameplayAbility_Montage::UPGAS_GameplayAbility_Montage()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// This function is called when the ability is activated
// It sets up the ability, checks if it can be committed, and plays the montage if everything is valid.
void UPGAS_GameplayAbility_Montage::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // Cache the parameters for later use
    CachedHandle = Handle;
    CachedActorInfo = ActorInfo;
    CachedActivationInfo = ActivationInfo;
    CachedTriggerEventData = TriggerEventData;  

    // Check if the montage to play is valid and if the ability can be committed
    if (!MontageToPlay || !CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        // If we can't commit the ability, end it immediately
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    if (MontageToPlay && ActorInfo && ActorInfo->AvatarActor.IsValid())
    {
        UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
        if (ASC)
        {
            UPGAS_AbilitySystemComponent* Custom_ASC = Cast<UPGAS_AbilitySystemComponent>(ASC);
            if (Custom_ASC)
            {
                // Set the current combo step in the ASC
                Custom_ASC->CurrentComboStep = ComboStep;
            }

            // Loop through the montage notifies and setup notifications we can trigger events from.
            for (const FAnimNotifyEvent& Notify : MontageToPlay->Notifies)
            {
                UPGAS_GameplayTagNotify* GameplayTagNotify = Cast<UPGAS_GameplayTagNotify>(Notify.NotifyStateClass);
                if (GameplayTagNotify)
                {
                    // Subscribe to the notify begin and end events by the GameplayTagNotify notify begin and end gameplay tags.
                    UAbilityTask_WaitGameplayEvent* WaitEventBegin = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GameplayTagNotify->BeginNotifyTag);
                    if (WaitEventBegin)
                    {
                        WaitEventBegin->EventReceived.AddDynamic(this, &UPGAS_GameplayAbility_Montage::OnNotifyBegin);
                        WaitEventBegin->ReadyForActivation();
                    }

                    UAbilityTask_WaitGameplayEvent* WaitEventEnd = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GameplayTagNotify->EndNotifyTag);
                    if (WaitEventEnd)
                    {
                        WaitEventEnd->EventReceived.AddDynamic(this, &UPGAS_GameplayAbility_Montage::OnNotifyEnd);
                        WaitEventEnd->ReadyForActivation();
                    }
                }
                else
                {
                    // Handle non-gameplay tag notifies (Regular notifiers)
                    MontageNotify(Notify.NotifyName); // Call the Blueprint event for the notify
                }
            }

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
                MontageTask->OnCompleted.AddDynamic(this, &UPGAS_GameplayAbility_Montage::OnMontageCompleted);
                MontageTask->OnInterrupted.AddDynamic(this, &UPGAS_GameplayAbility_Montage::OnMontageInterrupted);
                MontageTask->OnCancelled.AddDynamic(this, &UPGAS_GameplayAbility_Montage::OnMontageCancelled);
                MontageTask->ReadyForActivation(); // Start the task!
            }
        }
    }
}

// This function is called when the ability ends
// It cleans up the ability and unbinds any delegates to prevent stale bindings.
void UPGAS_GameplayAbility_Montage::EndAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// This function is called when the montage ends, either normally or interrupted
void UPGAS_GameplayAbility_Montage::OnMontageCompleted()
{
    // If the actor info is valid, reset the attacking state
    // This is important to ensure the character's state is consistent after completing the montage
    if (CachedActorInfo)
    {
        APGAS_CharacterBase* Character = Cast<APGAS_CharacterBase>(CachedActorInfo->OwnerActor);
        if (Character)
            Character->SetIsAttacking(false); // Reset the attacking state when the montage completes
    }
    
    // Notify the end of the montage
    MontageCompleted();

    // End the ability when the montage ends
    EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, false, false);
}

void UPGAS_GameplayAbility_Montage::OnMontageInterrupted()
{
    // If the actor info is valid, reset the attacking state
    // This is important to ensure the character's state is consistent after an interruption
    if (CachedActorInfo)
    {
        APGAS_CharacterBase* Character = Cast<APGAS_CharacterBase>(CachedActorInfo->OwnerActor);
        if (Character)
            Character->SetIsAttacking(false); // Reset the attacking state when the montage completes
    }

    // Notify the interruption of the montage
    MontageInterrupted();

    // End the ability when the montage is interrupted
    EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, false);
}

void UPGAS_GameplayAbility_Montage::OnMontageCancelled()
{
    // If the actor info is valid, reset the attacking state
    // This is important to ensure the character's state is consistent after an cancellation
    if (CachedActorInfo)
    {
        APGAS_CharacterBase* Character = Cast<APGAS_CharacterBase>(CachedActorInfo->OwnerActor);
        if (Character)
            Character->SetIsAttacking(false); // Reset the attacking state when the montage completes
    }

    // Notify the cancellation of the montage
    MontageCancelled();

    // End the ability when the montage is cancelled
    EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, false, false);
}

void UPGAS_GameplayAbility_Montage::OnNotifyBegin(FGameplayEventData Payload)
{
    OnMontageStateNotify.Broadcast(Payload.EventTag, Payload); // Broadcast it before Blueprint handling
    MontageStateNotify(Payload.EventTag, Payload); // Fire the montage state notify event for the begin notify
}

void UPGAS_GameplayAbility_Montage::OnNotifyEnd(FGameplayEventData Payload)
{
    OnMontageStateNotify.Broadcast(Payload.EventTag, Payload); // Broadcast it before Blueprint handling
    MontageStateNotify(Payload.EventTag, Payload); // Fire the montage state notify event for the end notify
}

void UPGAS_GameplayAbility_Montage::OnComboInputReceived()
{
    if (bComboWindowOpen)
    {
        // If the combo window is open, try to activate the next combo ability
        TryActivateNextCombo();
    }
}

void UPGAS_GameplayAbility_Montage::TryActivateNextCombo()
{
    // Check if the next combo ability is set
    if (NextComboAbility)
    {
        // End current ability and activate next step
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

        // Get the ability system component
        UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
        if (ASC)
        {
            // Try to activate the next combo ability
            ASC->TryActivateAbilityByClass(NextComboAbility);
        }
    }
}

// Static delegate instance for montage state notifications
// This allows other classes to bind to this delegate to receive montage state notifications
FOnMontageStateNotify UPGAS_GameplayAbility_Montage::OnMontageStateNotify;