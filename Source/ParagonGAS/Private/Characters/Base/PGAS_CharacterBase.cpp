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
	* Date: 6-28-2025
	* =============================================================================
*/


#include "Characters/Base/PGAS_CharacterBase.h"
#include <GAS/PGAS_AbilitySystemComponent.h>
#include "AbilitySystemGlobals.h"

// Sets default values
APGAS_CharacterBase::APGAS_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// // Set up the character's hit reaction component.
	// HitReactionComponent = CreateDefaultSubobject<UPGAS_HitReactionComponent>(TEXT("HitReactionComponent"));

	// Set up the character's ability system component.
	// This component is responsible for managing abilities and effects.
	AbilitySystemComponent = CreateDefaultSubobject<UPGAS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void APGAS_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

/// Adds a gameplay tag to this character
void APGAS_CharacterBase::AddGameplayTag(FGameplayTag Tag)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AddLooseGameplayTag(Tag);
	}
}

/// Removes a gameplay tag from this character
void APGAS_CharacterBase::RemoveGameplayTag(FGameplayTag Tag)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->RemoveLooseGameplayTag(Tag);
	}
}

/// Returns all the gameplay tags for this character
FGameplayTagContainer APGAS_CharacterBase::GetGameplayTags() const
{
	if (const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		FGameplayTagContainer OwnedTags;
		ASC->GetOwnedGameplayTags(OwnedTags);
		return OwnedTags; // Return the container filled with the ASC's tags
	}

	return FGameplayTagContainer();
}

/// Checks if this character has a specific gameplay tag
bool APGAS_CharacterBase::HasGameplayTag(FGameplayTag Tag) const
{
	// Check if the Ability System Component is valid and has the tag
	if (const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		return ASC->HasMatchingGameplayTag(Tag);
	}

	return false;
}

/// Checks if this character has all gameplay tags in the provided container
bool APGAS_CharacterBase::HasAllGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		return ASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

/// Checks if this character has any gameplay tags in the provided container
bool APGAS_CharacterBase::HasAnyGameplayTag(const FGameplayTagContainer& TagContainer) const
{
	if (const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		return ASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

// Activates abilities with the specified tags
bool APGAS_CharacterBase::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent)
	{
		return false;
	}

	// Activate abilities with the specified tags
	return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, AllowRemoteActivation);
}

// Applies default attribute effects to the character
void APGAS_CharacterBase::ApplyDefaultAttributeEffects()
{
	// Initialize the ability system component when the character is possessed
	if (AbilitySystemComponent)
	{
		for (TSubclassOf<class UGameplayEffect>& Effect : DefaultAttributeEffects)
		{
			if (Effect)
			{
				// Give the default attribute effect to the character
				FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, CharacterLevel, EffectContext);
				if (SpecHandle.IsValid())
				{
					FActiveGameplayEffectHandle ActiveEffect = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}
}

// Removes default attribute effects from the character
void APGAS_CharacterBase::RemoveDefaultAttributeEffects()
{
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	AbilitySystemComponent->RemoveActiveEffects(Query);
}
