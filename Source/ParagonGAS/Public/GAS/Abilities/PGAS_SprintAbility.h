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

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Sound/SoundCue.h"
#include "PGAS_SprintAbility.generated.h"

/**
 * 
 */
UCLASS()
class PARAGONGAS_API UPGAS_SprintAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPGAS_SprintAbility();

	// Called when the ability is activated
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	// Called when the ability ends or is cancelled
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

protected:
	// Sprint speed to apply (exposed for tuning in BP or DT)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint")
	float SprintSpeed = 600.f;

	// Normal walk speed to restore (cached at activate)
	UPROPERTY(Transient)
	float WalkSpeed = 350.f;

	// Optionally, add a GameplayTag to restrict when you can jump (e.g. "State.CanJump")
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag RequiredTag;

	// Optionally, add a sound cue to play when sprinting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Audio")
	TObjectPtr<USoundBase> SprintSoundCue;

	// Gameplay Effect to apply when sprinting (e.g., stamina reduction)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Cost")
	TSubclassOf<class UGameplayEffect> StaminaReductionEffect;
};
