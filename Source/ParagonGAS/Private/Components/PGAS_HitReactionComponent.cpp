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


#include "Components/PGAS_HitReactionComponent.h"
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
}

// Called every frame
void UPGAS_HitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/**
 * Gets the owning character of this component.
 * @return The character that owns this component, or nullptr if not found.
*/
ACharacter* UPGAS_HitReactionComponent::GetOwningCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

/**
 * Performs a hit reaction based on the specified hit direction.
 * @param HitDirection The direction of the hit (e.g., "Hit.Back", "Hit.Forward", etc.).
 * @param PlayRate The play rate for the hit reaction montage.
*/
void UPGAS_HitReactionComponent::PerformHitReaction(const FGameplayTag& HitDirection, float PlayRate)
{
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
	if (Montage && GetOwningCharacter())
	{
		UAnimInstance* AnimInstance = GetOwningCharacter()->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(Montage, InPlayRate);
		}
	}
}
