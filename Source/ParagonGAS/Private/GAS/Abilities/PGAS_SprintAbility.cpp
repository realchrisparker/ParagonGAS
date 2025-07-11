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
 * Date: 7-11-2025
 * =============================================================================
 * PGAS_SprintAbility
 * This ability toggles sprint for the player character using GAS. It sets MaxWalkSpeed on activate and restores it on end.
 */


#include <GAS/Abilities/PGAS_SprintAbility.h>
#include <GAS/Effects/PGAS_GE_StaminaReduction.h>
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"

UPGAS_SprintAbility::UPGAS_SprintAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    bReplicateInputDirectly = true;

    // Ability Tag for matching Event
    SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Character.Ability.Sprint"))));

    ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove")));

    StaminaReductionEffect = UPGAS_GE_StaminaReduction::StaticClass();
}

void UPGAS_SprintAbility::ActivateAbility(
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

    // Play the sound (if set)
    if (SprintSoundCue && ActorInfo && ActorInfo->AvatarActor.IsValid())
    {
        UGameplayStatics::PlaySoundAtLocation(
            ActorInfo->AvatarActor->GetWorld(),
            SprintSoundCue,
            ActorInfo->AvatarActor->GetActorLocation());
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character && Character->GetCharacterMovement())
    {
        // Cache the original speed to restore later
        Character->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }

    // If using input, keep ability active until input is released
    SetCanBeCanceled(true);
    SetShouldBlockOtherAbilities(false);

    if (StaminaReductionEffect && ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
    {
        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(StaminaReductionEffect, GetAbilityLevel());
        SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Combat.Stamina.Reduction")), -0.120f); // Set the magnitude
        ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
}

void UPGAS_SprintAbility::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character && Character->GetCharacterMovement())
    {
        Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}