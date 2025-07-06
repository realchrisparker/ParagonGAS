/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * This ability listens for the Combat.Damage.Event.Melee.Staff tag and handles applying damage.
 * Author: Christopher D. Parker
 * Date: 7-4-2025
 * =============================================================================
 * PGAC_ReceiveStaffMeleeDamageAbility
 * This ability listens for the Combat.Damage.Event.Melee.Staff tag and handles applying damage.
 */

#include "GAS/Abilities/PGAS_ReceiveStaffMeleeDamageAbility.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
// #include <Objects/PGAS_EventDataOptionalObj.h>
#include <GAS/Effects/PGAS_GE_StaffMeleeDamageEffect.h>
#include "Characters/Enemy/PGAS_EnemyCharacter.h"


UPGAS_ReceiveStaffMeleeDamageAbility::UPGAS_ReceiveStaffMeleeDamageAbility()
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

void UPGAS_ReceiveStaffMeleeDamageAbility::ActivateAbility(
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
            APGAS_EnemyCharacter* TargetCharacter = Cast<APGAS_EnemyCharacter>(TargetActor);
            if (!TargetCharacter)
            {
                // No character base found, log and end ability
                UE_LOG(LogTemp, Warning, TEXT("Target actor is not a PGAS_EnemyCharacter: %s"), *TargetActor->GetName());
                EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
                return;
            }

            // Calculate if the attack is a crit (example: 15% chance)
            bool bIsCritical = (FMath::FRand() < 0.15f);

            // Use the StaticClass for the GE
            TSubclassOf<UGameplayEffect> DamageEffectClass = UPGAS_GE_StaffMeleeDamageEffect::StaticClass();
            FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, TargetCharacter->GetCharacterLevel());

            // If you want to access default crit multiplier etc, use GetDefault<> (read-only)
            const UPGAS_GE_StaffMeleeDamageEffect* DamageEffect = Cast<const UPGAS_GE_StaffMeleeDamageEffect>(DamageEffectClass->GetDefaultObject());
            float CritMultiplier = DamageEffect && DamageEffect->bCanCrit ? DamageEffect->CritMultiplier : 1.0f;

            float BaseDamage = DamageEffect->BaseDamage;
            float FinalDamage = bIsCritical ? BaseDamage * CritMultiplier : BaseDamage;

            // SetByCaller for dynamic runtime values
            EffectSpecHandle.Data->SetSetByCallerMagnitude(EventTriggerTag, -FinalDamage);

            // Set context (instigator)
            EffectSpecHandle.Data->GetContext().AddInstigator(ActorInfo->OwnerActor.Get(), ActorInfo->AvatarActor.Get());

            UE_LOG(LogTemp, Log, TEXT("Target current health %f"), TargetCharacter->GetHealth());

            // Apply the effect to the target
            TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

            UE_LOG(LogTemp, Log, TEXT("Target after damage health %f"), TargetCharacter->GetHealth());
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPGAS_ReceiveStaffMeleeDamageAbility::EndAbility(
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
