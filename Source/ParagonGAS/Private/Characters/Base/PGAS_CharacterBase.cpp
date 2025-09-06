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

#include <Characters/Base/PGAS_CharacterBase.h>
#include <GAS/PGAS_AbilitySystemComponent.h>
#include <Interfaces/IAnimation.h>
#include "AbilitySystemGlobals.h"
#include "Components/PoseableMeshComponent.h"

// Sets default values
APGAS_CharacterBase::APGAS_CharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the hit reaction component.
	// This component handles hit reactions for the enemy character.
	HitReactionComponent = CreateDefaultSubobject<UPGAS_HitReactionComponent>(TEXT("Hit Reaction Component"));

	// Set up the character's ability system component.
	// This component is responsible for managing abilities and effects.
	AbilitySystemComponent = CreateDefaultSubobject<UPGAS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void APGAS_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	ApplyStartingGameplayTags(); // Apply starting gameplay tags
}

void APGAS_CharacterBase::ApplyStartingGameplayTags()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AddLooseGameplayTags(StartingGameplayTags);
	}
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

/*
 * Activates abilities with the specified gameplay tags.
 * @param AbilityTags The gameplay tags of the abilities to activate.
 * @param AllowRemoteActivation Whether to allow remote activation of abilities (default: true).
 * @return bool True if any abilities were activated, false otherwise.
*/
bool APGAS_CharacterBase::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent)
	{
		return false;
	}

	// Activate abilities with the specified tags
	return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, AllowRemoteActivation);
}

// Activates abilities with the specified tags
void APGAS_CharacterBase::CancelAbilitiesWithTags(FGameplayTagContainer AbilityTags)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Cancel abilities with the specified tags
	AbilitySystemComponent->CancelAbilities(&AbilityTags, nullptr, nullptr);
}

// Applies default attribute effects to the character
void APGAS_CharacterBase::ApplyDefaultAttributeEffects()
{
	// Initialize the ability system component when the character is possessed
	if (AbilitySystemComponent && HasAuthority())
	{
		// Loop through the DefaultAttributeEffects array and apply each effect
		// This ensures that effects are only applied once, even if this function is called multiple times
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

// Sets up default abilities for the enemy character.
// This function can be used to set up default abilities for the enemy character.
void APGAS_CharacterBase::SetupDefaultAbilities()
{
	if (AbilitySystemComponent && HasAuthority())
	{
		// Look through the DefaultAbilities array and grant each ability if it hasn't already been granted
		// This ensures that abilities are only granted once, even if this function is called multiple times
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
		{
			if (!AbilityClass)
				continue;

			bool bAlreadyGranted = false;
			for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
			{
				if (Spec.Ability && Spec.Ability->GetClass() == AbilityClass)
				{
					bAlreadyGranted = true;
					break;
				}
			}
			if (!bAlreadyGranted)
			{
				// Determine if this is an ability that needs to use the Enhanced Input System
				const UGameplayAbility* DefaultAbilityCDO = AbilityClass->GetDefaultObject<UGameplayAbility>();
				if (DefaultAbilityCDO && DefaultAbilityCDO->GetAssetTags().HasTagExact(FGameplayTag::RequestGameplayTag("Character.Ability.Sprint")))
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, (int32)EPGAS_AbilityInputID::Sprint));
				}
				else if (DefaultAbilityCDO && DefaultAbilityCDO->GetAssetTags().HasTagExact(FGameplayTag::RequestGameplayTag("Character.Ability.Block")))
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, (int32)EPGAS_AbilityInputID::Block));
				}
				else
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE));
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

// Starts blocking (Helper function)
void APGAS_CharacterBase::StartBlocking()
{
	// Make sure we have an anim instance.
	if (UAnimInstance* AnimInst = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		// Make sure our anim instance implements the IAnimation interface
		if (IIAnimation* AnimInterface = Cast<IIAnimation>(AnimInst))
		{
			AnimInterface->StartBlocking(); // Starts blocking
		}
	}
}

// Stops blocking (Helper function)
void APGAS_CharacterBase::StopBlocking()
{
	// Make sure we have an anim instance.
	if (UAnimInstance* AnimInst = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		// Make sure our anim instance implements the IAnimation interface
		if (IIAnimation* AnimInterface = Cast<IIAnimation>(AnimInst))
		{
			AnimInterface->StopBlocking(); // Stops blocking
		}
	}
}

/*
 * Display a poseable mesh of this character at their current location/orientation.
 * This is used for dodge shadowing effects, etc.
 */
void APGAS_CharacterBase::ShowCharacterSnapshot(float Duration)
{
	if (!GetMesh()) return;

	// Destroy previous snapshot if one exists
	if (ActiveShadowPoseableMesh && IsValid(ActiveShadowPoseableMesh))
	{
		ActiveShadowPoseableMesh->DestroyComponent();
		ActiveShadowPoseableMesh = nullptr;
	}

	UPoseableMeshComponent* NewShadow = NewObject<UPoseableMeshComponent>(this, UPoseableMeshComponent::StaticClass(), NAME_None);

	if (NewShadow)
	{
		// Register the component so it becomes active in the world
		NewShadow->RegisterComponent();

		// Place it at the current transform of the player mesh
		NewShadow->SetWorldTransform(GetMesh()->GetComponentTransform());

		// Copy skeletal mesh and frozen pose
		NewShadow->SetSkinnedAssetAndUpdate(GetMesh()->GetSkeletalMeshAsset());
		NewShadow->CopyPoseFromSkeletalComponent(GetMesh());

		// Save reference
		ActiveShadowPoseableMesh = NewShadow;

		// Destroy after given duration
		FTimerHandle DestroyHandle;
		GetWorldTimerManager().SetTimer(
			DestroyHandle,
			FTimerDelegate::CreateLambda([ this, NewShadow ] ()
				{
					if (IsValid(NewShadow))
					{
						NewShadow->DestroyComponent();
						if (ActiveShadowPoseableMesh == NewShadow)
						{
							ActiveShadowPoseableMesh = nullptr;
						}
					}
				}),
			Duration,
			false
		);
	}
}