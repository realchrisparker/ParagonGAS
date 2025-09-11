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
 * Date: 7-12-2025
 * =============================================================================
 * PGAS_HitReactionComponent
 * Handles hit reaction logic for actors (flinch, stagger, hit animations, VFX, etc.).
*/


#include <Components/PGAS_CombatHitReactionComponent.h>
#include <Characters/Base/PGAS_CharacterBase.h>
#include <Data/PGAS_HitReactionDataObject.h>
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UPGAS_CombatHitReactionComponent::UPGAS_CombatHitReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPGAS_CombatHitReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache the owner character for quick access
	// This assumes the owner is a character, you can adjust if needed
	CachedOwnerCharacter = Cast<APGAS_CharacterBase>(GetOwner());
}

/**
 * Performs a hit reaction based on the specified hit direction.
 * @param HitDirection The direction of the hit (e.g., "Hit.Back", "Hit.Forward", etc.).
 * @param PlayRate The play rate for the hit reaction montage.
*/
void UPGAS_CombatHitReactionComponent::PerformHitReaction(EPGAS_HitDirection Direction, EPGAS_HitSeverity Severity)
{
	if (!CachedOwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitReactionComponent: No owner character."));
		return;
	}

	// Find the best reaction
	const FPGAS_HitReaction* Reaction = FindReaction(Direction, Severity);

	if(!Reaction)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitReactionComponent: No matching hit reaction found."));
		return;
	}

	// Activate ability or play montage
	// Abilities take precedence over montages
	if (Reaction->AbilityTag.IsValid())
	{
		// If the reaction has an ability tag, try to activate that ability by event
		UPGAS_HitReactionDataObject* DataObj = NewObject<UPGAS_HitReactionDataObject>();
		DataObj->Data = *Reaction;

		// Build event data
		FGameplayEventData EventData;
		EventData.EventTag = Reaction->AbilityTag; // ability listens for this
		EventData.OptionalObject = DataObj;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedOwnerCharacter, Reaction->AbilityTag, EventData);
	}
	else if (Reaction->Montage) // Play the montage if found
	{
		PlayMontageInternal(Reaction->Montage, Reaction->PlayRate);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HitReactionComponent: No matching hit reaction found for %d + %d"),
			(int32)Direction, (int32)Severity);
	}
}

/**
 * Finds the best matching reaction based on direction and severity.
 * @param Direction The hit direction.
 * @param Severity The hit severity.
 * @return The best matching hit reaction, or nullptr if none found.
 */
const FPGAS_HitReaction* UPGAS_CombatHitReactionComponent::FindReaction(EPGAS_HitDirection Direction, EPGAS_HitSeverity Severity) const
{
	// First look for exact match
	for (const FPGAS_HitReaction& Reaction : HitReactions)
	{
		if (Reaction.Direction == Direction && Reaction.Severity == Severity)
		{
			return &Reaction;
		}
	}

	// If none found, fallback: try matching only by severity
	for (const FPGAS_HitReaction& Reaction : HitReactions)
	{
		if (Reaction.Severity == Severity)
		{
			return &Reaction;
		}
	}

	// If still none found, fallback: try matching only by direction
	for (const FPGAS_HitReaction& Reaction : HitReactions)
	{
		if (Reaction.Direction == Direction)
		{
			return &Reaction;
		}
	}

	// Nothing found
	return nullptr;
}

/**
 * Helper: Plays a montage on the owning actor's mesh if valid
 * @param Montage The montage to play
 * @param InPlayRate The play rate for the montage
 */
void UPGAS_CombatHitReactionComponent::PlayMontageInternal(UAnimMontage* Montage, float InPlayRate)
{
	if (Montage && CachedOwnerCharacter)
	{
		if (UAnimInstance* AnimInstance = CachedOwnerCharacter->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(Montage, InPlayRate);
		}
	}
}
