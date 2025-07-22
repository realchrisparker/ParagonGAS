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


#include <Components/PGAS_HitReactionComponent.h>
#include <Characters/Base/PGAS_CharacterBase.h>
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UPGAS_HitReactionComponent::UPGAS_HitReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPGAS_HitReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache the owner character for quick access
	// This assumes the owner is a character, you can adjust if needed
	CachedOwnerCharacter = Cast<APGAS_CharacterBase>(GetOwner());
}

// Called every frame
void UPGAS_HitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/**
 * Performs a hit reaction based on the specified hit direction.
 * @param HitDirection The direction of the hit (e.g., "Hit.Back", "Hit.Forward", etc.).
 * @param PlayRate The play rate for the hit reaction montage.
*/
void UPGAS_HitReactionComponent::PerformHitReaction(const FGameplayTag& HitDirection, float PlayRate)
{
	// Check if we have a valid owner character
	if (!CachedOwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("No owning character found"));
		return;
	}

	// Check if the character is invincible (Don't have to get the ASC here, we can use the character directly)
	if (CachedOwnerCharacter->HasGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Invincible"))))
		return;

	// Play the appropriate hit reaction montage based on the hit direction
	if (HitDirection == FGameplayTag::RequestGameplayTag(FName("Hit.Reactions.Back")))
	{
		if (BackHitMontage)
		{
			PlayMontageInternal(BackHitMontage, PlayRate);
			return;
		}
	}
	else if (HitDirection == FGameplayTag::RequestGameplayTag(FName("Hit.Reactions.Forward")))
	{
		if (ForwardHitMontage)
		{
			PlayMontageInternal(ForwardHitMontage, PlayRate);
			return;
		}
	}
	else if (HitDirection == FGameplayTag::RequestGameplayTag(FName("Hit.Reactions.Left")))
	{
		if (LeftHitMontage)
		{
			PlayMontageInternal(LeftHitMontage, PlayRate);
			return;
		}
	}
	else if (HitDirection == FGameplayTag::RequestGameplayTag(FName("Hit.Reactions.Right")))
	{
		if (RightHitMontage)
		{
			PlayMontageInternal(RightHitMontage, PlayRate);
			return;
		}
	}

	if (DefaultHitMontage)
	{
		// If no specific montage is found for the hit direction, play the default hit montage
		PlayMontageInternal(DefaultHitMontage, PlayRate);
	}
}

/**
 * Plays the specified hit reaction montage.
 * @param Montage The montage to play.
 * @param InPlayRate The play rate for the montage.
 */
void UPGAS_HitReactionComponent::PlayMontageInternal(UAnimMontage* Montage, float InPlayRate)
{
	if (Montage && CachedOwnerCharacter)
	{
		UAnimInstance* AnimInstance = CachedOwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(Montage, InPlayRate);
		}
	}
}
