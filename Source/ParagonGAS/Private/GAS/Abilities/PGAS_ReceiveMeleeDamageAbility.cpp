/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * This ability listens for the Combat.Damage.Event.Melee tag and handles applying damage.
 * Author: Christopher D. Parker
 * Date: 7-4-2025
 * =============================================================================
 * PGAS_GE_MeleeDamageEffect
 * This ability listens for the Combat.Damage.Event.Melee tag and handles applying damage.
 */

#include "GAS/Abilities/PGAS_ReceiveMeleeDamageAbility.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include <Data/PGAS_EventAdditionalData.h>
#include <GAS/Effects/PGAS_GE_MeleeDamageEffect.h>
#include "Characters/Base/PGAS_CharacterBase.h"


UPGAS_ReceiveMeleeDamageAbility::UPGAS_ReceiveMeleeDamageAbility()
{
    // This ability can only be triggered by an event
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Ability Tag for matching Event
    SetAssetTags(FGameplayTagContainer(EventTriggerTag));

    // This allows this ability to be activated by the event tag
    ActivationOwnedTags.AddTag(EventTriggerTag);

    // Set up the ability trigger data to listen for the damage event
    FAbilityTriggerData TriggerData;
    TriggerData.TriggerTag = EventTriggerTag;
    TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    AbilityTriggers.Add(TriggerData);
}

void UPGAS_ReceiveMeleeDamageAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData
)
{
    if (TriggerEventData)
    {
        // Cast to your enemy character type if needed
        if (AActor* TargetActor = Cast<AActor>(ActorInfo->AvatarActor.Get()))
        {
            // Get the target's Ability System Component
            UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
            if (!TargetASC)
            {
                // No Ability System Component found, log and end ability
                UE_LOG(LogTemp, Warning, TEXT("No Ability System Component found on target actor: %s"), *TargetActor->GetName());
                EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
                return;
            }

            // Cast to your character base class if needed
            // This assumes the target actor is a character that has health and can take damage
            APGAS_CharacterBase* TargetCharacter = Cast<APGAS_CharacterBase>(TargetActor);
            if (!TargetCharacter)
            {
                // No character base found, log and end ability
                UE_LOG(LogTemp, Warning, TEXT("Target actor is not a PGAS_CharacterBase: %s"), *TargetActor->GetName());
                EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
                return;
            }

            // Calculate if the attack is a crit (example: 15% chance)
            bool bIsCritical = (FMath::FRand() < 0.15f);

            // Use the StaticClass for the GE
            TSubclassOf<UGameplayEffect> DamageEffectClass = UPGAS_GE_MeleeDamageEffect::StaticClass();
            FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, TargetCharacter->GetCharacterLevel());

            // Set context (instigator)
            EffectSpecHandle.Data->GetContext().AddInstigator(ActorInfo->OwnerActor.Get(), ActorInfo->AvatarActor.Get());

            // Apply the effect to the target
            TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

            // Get hit result from event data if available
            FHitResult HitResult;
            if (TriggerEventData->OptionalObject)
            {
                const UPGAS_EventAdditionalData* HitReactionData = Cast<UPGAS_EventAdditionalData>(TriggerEventData->OptionalObject);
                if (HitReactionData)
                {
                    HitResult = HitReactionData->HitResult;
                    FGameplayTag HitDirectionTag = FGameplayTag::RequestGameplayTag(FName("Hit.Reactions.Left"));

                    // Determine hit direction based on the hit result
                    if (HitResult.ImpactNormal.Z > 0.5f)
                    {
                        HitDirectionTag = FGameplayTag::RequestGameplayTag(FName("Hit.Reactions.Back"));
                    }
                    else if (HitResult.ImpactNormal.Z < -0.5f)
                    {
                        HitDirectionTag = FGameplayTag::RequestGameplayTag(FName("Hit.Reactions.Forward"));
                    }
                    else if (HitResult.ImpactNormal.X > 0.5f)
                    {
                        HitDirectionTag = FGameplayTag::RequestGameplayTag(FName("Hit.Reactions.Right"));
                    }
                    else if (HitResult.ImpactNormal.X < -0.5f)
                    {
                        HitDirectionTag = FGameplayTag::RequestGameplayTag(FName("Hit.Reactions.Left"));
                    }

                    // Send hit reaction request to the target character's hit reaction component
                    if (TargetCharacter->GetHitReactionComponent())
                    {
                        TargetCharacter->GetHitReactionComponent()->PerformHitReaction(HitDirectionTag, 1.0f);
                    }
                }
            }
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPGAS_ReceiveMeleeDamageAbility::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled
)
{
    // Clean up or reset any state if necessary
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
