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
 * Implementation of the base Gameplay Ability class.
 */

#include "GAS/Abilities/PGAS_GameplayAbilityBase.h"

/** Constructor */
UPGAS_GameplayAbilityBase::UPGAS_GameplayAbilityBase()
{
	// Default to instancing the ability per actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

/**
 * Called when the ability is assigned to an avatar.
 * We use it to cache references to the character and its components.
 */
void UPGAS_GameplayAbilityBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		CachedCharacter = Cast<APGAS_CharacterBase>(ActorInfo->AvatarActor.Get());

		if (CachedCharacter)
		{
			CachedCharacterLevel = CachedCharacter->GetCharacterLevel();
			CachedCharacterMovement = CachedCharacter->GetCharacterMovement();
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
void UPGAS_GameplayAbilityBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// Always call the parent implementation first
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Optional: add custom code that all abilities should run on activation
}

/**
 * Called when the ability ends.
 * @param Handle The handle of the ability spec.
 * @param ActorInfo The actor info for the ability.
 * @param ActivationInfo The activation info for the ability.
 * @param bReplicateEndAbility Whether to replicate the end ability to other clients.
 * @param bWasCancelled Whether the ability was cancelled.
 */
void UPGAS_GameplayAbilityBase::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	// Optional: add custom code that all abilities should run on end

	// Call parent to properly clean up
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
