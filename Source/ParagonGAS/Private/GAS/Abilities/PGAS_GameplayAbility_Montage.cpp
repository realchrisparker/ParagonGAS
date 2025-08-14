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
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Data/PGAS_EventAdditionalData.h>
#include <Weapons/Base/PGAS_WeaponBase.h>


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

    // Determine if this is begin damage notify (Attack.Damage.Notify.Begin)
    if (Payload.EventTag == FGameplayTag::RequestGameplayTag(TEXT("Attack.Damage.Notify.Begin")))
    {
        // Determine if this is an enemy character
        if (AActor* AvatarActor = GetAvatarActorFromActorInfo())
        {
            AvatarActor->GetWorldTimerManager().SetTimer(
                WeaponTraceTimerHandle,
                this,
                &UPGAS_GameplayAbility_Montage::WeaponTrace,
                0.01f, // Every 0.01 seconds
                true   // Loop
            );
        }
    }
}

void UPGAS_GameplayAbility_Montage::OnNotifyEnd(FGameplayEventData Payload)
{
    OnMontageStateNotify.Broadcast(Payload.EventTag, Payload); // Broadcast it before Blueprint handling
    MontageStateNotify(Payload.EventTag, Payload); // Fire the montage state notify event for the end notify

    // Determine if this is end damage notify (Attack.Damage.Notify.End)
    if (Payload.EventTag == FGameplayTag::RequestGameplayTag(TEXT("Attack.Damage.Notify.End")))
    {
        // Determine if this is an enemy character
        if (AActor* AvatarActor = GetAvatarActorFromActorInfo())
        {
            AvatarActor->GetWorldTimerManager().ClearTimer(WeaponTraceTimerHandle);
        }
    }
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

/*
 * Performs a weapon trace to detect hits.
 * This function should be implemented to perform a trace from Start to End.
 * @return void.
*/
void UPGAS_GameplayAbility_Montage::WeaponTrace()
{
    UWorld* World = GetWorld();
    if (!World) return;

    // Get the enemycharacter from CachedActorInfo
    APGAS_EnemyCharacter* Character = Cast<APGAS_EnemyCharacter>(CachedActorInfo->OwnerActor);
    if (Character && Character->EquippedWeapon)
    {
        float SphereRadius = 25.0f;
        
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

        TArray<AActor*> IgnoreActors;
        IgnoreActors.Add(Character); // Ignore self in the trace

        TArray<FHitResult> OutHits;
        bool bHit = Character->EquippedWeapon->SphereTraceBlade(SphereRadius, ObjectTypes, IgnoreActors, OutHits, false);
        if (bHit)
        {
            for (const FHitResult& Hit : OutHits)
            {
                // Get the actor that was hit.
                AActor* HitActor = Hit.GetActor();
                if (!HitActor) continue;

                // Check if the hit actor is valid and not already hit in this window.
                if (AlreadyHitActors.Contains(HitActor)) continue;

                // Actor not hit yet, so we add it to the set of already hit actors.
                AlreadyHitActors.Add(HitActor);

                // Prepare the event tag (must match what your abilities expect)
                FGameplayTag DamageEventTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Melee"));

                // Create Instigator Tags
                FGameplayTagContainer InstigatorTags = Character->GetGameplayTags(); // Get the character's gameplay tags at this moment

                // Create optional object to hold hit information
                UPGAS_EventAdditionalData* EvnDataOptObj = NewObject<UPGAS_EventAdditionalData>();
                EvnDataOptObj->HitResult = Hit;

                // Prepare the event data
                FGameplayEventData EventData;
                EventData.EventTag = DamageEventTag; // The event tag for the damage event
                EventData.OptionalObject = EvnDataOptObj; // Optional object to hold additional data
                EventData.Instigator = Character; // The actor that initiated the event (this character)
                EventData.InstigatorTags = InstigatorTags; // Tags from the instigator
                EventData.Target = HitActor; // The actor that was hit

                // Send the event
                UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, DamageEventTag, EventData);
            }
        }
    }
}

// Static delegate instance for montage state notifications
// This allows other classes to bind to this delegate to receive montage state notifications
FOnMontageStateNotify UPGAS_GameplayAbility_Montage::OnMontageStateNotify;