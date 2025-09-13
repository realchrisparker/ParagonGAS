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
 * Date: 8-17-2025
 * =============================================================================
 * Combat Core Component (Dual-Weapon Ready) - Implementation
 */

#include <Components/PGAS_CombatCoreComponent.h>
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include <Characters/Player/PGAS_PlayerCharacter.h>

// Constructor
UPGAS_CombatCoreComponent::UPGAS_CombatCoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPGAS_CombatCoreComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<APGAS_CharacterBase>(GetOwner()); // Cache the character owner
	if (CachedCharacter)
    {
		CachedASC = CachedCharacter->GetAbilitySystemComponent(); // Cache the Ability System Component from the character

		// Register gameplay tag events
		if (CachedASC)
		{
			// Register attack tags by looping through AttackTypes array.
			for (const FPGAS_AttackType& Attack : AttackTypes)
			{
				if (Attack.AbilityTag.IsValid())
				{
					FDelegateHandle Handle = CachedASC->RegisterGameplayTagEvent(
						Attack.AbilityTag,
						EGameplayTagEventType::NewOrRemoved
					).AddUObject(this, &UPGAS_CombatCoreComponent::HandleAttackWindowTagChanged);

					RegisteredTagHandles.Add(Handle);

					// UE_LOG(LogTemp, Warning, TEXT("Registered CombatCoreComponent for attack tag: %s (Hand: %d)"),
					// 	*Attack.AbilityTag.ToString(),
					// 	static_cast<int32>(Attack.Hand));
				}
			}

			// Register combo tag if valid. Only need to register once.
			if (ComboTag.IsValid()) {
				FDelegateHandle Handle = CachedASC->RegisterGameplayTagEvent(
					ComboTag,
					EGameplayTagEventType::NewOrRemoved
				).AddUObject(this, &UPGAS_CombatCoreComponent::HandleComboWindowTagChanged);

				RegisteredTagHandles.Add(Handle);

				// UE_LOG(LogTemp, Warning, TEXT("Registered CombatCoreComponent for combo tag: %s"), *ComboTag.ToString());
			}
		}
	}

	CurrentComboIndex = 0; // Initialize combo index
}

// Called when the component is destroyed or game ends
void UPGAS_CombatCoreComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CachedASC)
	{
		for (const FDelegateHandle& Handle : RegisteredTagHandles)
		{
			CachedASC->UnregisterGameplayTagEvent(Handle, FGameplayTag(), EGameplayTagEventType::NewOrRemoved);
		}
	}

	RegisteredTagHandles.Empty();

	Super::EndPlay(EndPlayReason);
}

// ---------------------------
// Ability helpers
// ---------------------------

/**
 * Activate an ability by tag. Optionally stamps a brief hand-context tag
 * (e.g., Combat.Hand.Left/Right) so the ability can branch on the active hand.
 */
bool UPGAS_CombatCoreComponent::ActivateAbilityByTag(const FGameplayTag& AbilityTag)
{
	if (!CachedCharacter || !AbilityTag.IsValid())
	{
		return false;
	}

	// Find attack data from the array
	const FPGAS_AttackType* FoundAttack = AttackTypes.FindByPredicate(
		[ & ] (const FPGAS_AttackType& Attack)
		{
			return Attack.AbilityTag == AbilityTag;
		});

	if (!FoundAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActivateAbilityByTag: No attack found for tag %s"), *AbilityTag.ToString());
		return false;
	}

	// --- Handle Combo Progression ---
	if (bComboWindowOpen)
	{
		// If we are in a combo window and player pressed attack again, advance combo
		CurrentComboIndex++;

		// Try to get the next attack in sequence
		FPGAS_AttackType NextAttack = GetNextComboAttack(
			FoundAttack->AttackType,
			CurrentComboIndex
		);

		if (NextAttack.AbilityTag.IsValid())
		{
			LastKnownAttack = NextAttack;

			FGameplayTagContainer Tags;
			Tags.AddTag(NextAttack.AbilityTag);

			bool bSuccess = CachedCharacter->ActivateAbilitiesWithTags(Tags, true);

			// Restart reset timer
			if (bSuccess)
			{
				GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);
				GetWorld()->GetTimerManager().SetTimer(
					ComboResetTimer,
					this,
					&UPGAS_CombatCoreComponent::ResetCombo,
					ComboResetTime,
					false
				);
			}

			return bSuccess;
		}
		else
		{
			// No next step found → reset combo
			ResetCombo();
			return false;
		}
	}
	else
	{
		// Cache last known info
		LastKnownAttack = *FoundAttack;

		// Build tag container for activation
		FGameplayTagContainer Tags;
		Tags.AddTag(AbilityTag);

		bool bSuccess = CachedCharacter->ActivateAbilitiesWithTags(Tags, true);

		// 🔥 Start reset timer for first step
		if (bSuccess)
		{
			GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);
			GetWorld()->GetTimerManager().SetTimer(
				ComboResetTimer,
				this,
				&UPGAS_CombatCoreComponent::ResetCombo,
				ComboResetTime,
				false
			);
		}

		return bSuccess;
	}
}

/**
 * Cancel abilities by tag.
 * @param CombatTag The combat tag type to cancel abilities for.
 */
void UPGAS_CombatCoreComponent::CancelAbilityByTag(const FGameplayTag& AbilityTag)
{
	if (!CachedCharacter || !AbilityTag.IsValid())
	{
		return;
	}

	// Find attack data from the array
	const FPGAS_AttackType* FoundAttack = AttackTypes.FindByPredicate(
		[ & ] (const FPGAS_AttackType& Attack)
		{
			return Attack.AbilityTag == AbilityTag;
		});

	if (!FoundAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("CancelAbilityByTag: No attack found for tag %s"), *AbilityTag.ToString());
		return;
	}

	// Build tag container for cancellation
	FGameplayTagContainer Tags;
	Tags.AddTag(AbilityTag);

	CachedCharacter->CancelAbilitiesWithTags(Tags);
}

// ---------------------------
// Windowing
// ---------------------------

/** Handle changes to the attack window tag */
void UPGAS_CombatCoreComponent::HandleAttackWindowTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	// Find the matching attack entry
	const FPGAS_AttackType* FoundAttack = AttackTypes.FindByPredicate(
		[ & ] (const FPGAS_AttackType& Attack)
		{
			return Attack.AbilityTag == Tag;
		});

	if (!FoundAttack)
	{
		// UE_LOG(LogTemp, Warning, TEXT("CombatCoreComponent: Received event for unregistered tag %s"), *Tag.ToString());
		return;
	}

	if (NewCount > 0)
	{
		OnCombatWindowOpen.Broadcast(*FoundAttack);
		// UE_LOG(LogTemp, Log, TEXT("Combat Window Opened: %s | Hand: %d | Weapon: %s"),
			// *FoundAttack->AbilityTag.ToString(),
			// static_cast<int32>(FoundAttack->Hand),
			// FoundAttack->WeaponData ? *FoundAttack->WeaponData->GetName() : TEXT("None"));
	}
	else
	{
		OnCombatWindowClose.Broadcast(*FoundAttack);
		// UE_LOG(LogTemp, Log, TEXT("Combat Window Closed: %s | Hand: %d | Weapon: %s"),
			// *FoundAttack->AbilityTag.ToString(),
			// static_cast<int32>(FoundAttack->Hand),
			// FoundAttack->WeaponData ? *FoundAttack->WeaponData->GetName() : TEXT("None"));
	}
}

void UPGAS_CombatCoreComponent::HandleComboWindowTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		OnComboWindowOpen.Broadcast(LastKnownAttack);
		bComboWindowOpen = true;
		// UE_LOG(LogTemp, Log, TEXT("Combo Window Opened: %s"), *Tag.ToString());
	}
	else
	{
		OnComboWindowClose.Broadcast(LastKnownAttack);
		bComboWindowOpen = false;
		// UE_LOG(LogTemp, Log, TEXT("Combo Window Closed: %s"), *Tag.ToString());
	}
}

// ---------------------------
// Attack Lookup Helpers
// ---------------------------

/** Return the first attack matching an AttackType (Light, Heavy, etc.) */
FPGAS_AttackType UPGAS_CombatCoreComponent::GetFirstAttackByType(EPGAS_WeaponAttackType InType) const
{
	const FPGAS_AttackType* Found = AttackTypes.FindByPredicate(
		[ & ] (const FPGAS_AttackType& Attack)
		{
			return Attack.AttackType == InType;
		});

	return Found ? *Found : FPGAS_AttackType();
}

/** Return all attacks matching an AttackType */
TArray<FPGAS_AttackType> UPGAS_CombatCoreComponent::GetAllAttacksByType(EPGAS_WeaponAttackType InType) const
{
	TArray<FPGAS_AttackType> Results;
	for (const FPGAS_AttackType& Attack : AttackTypes)
	{
		if (Attack.AttackType == InType)
		{
			Results.Add(Attack);
		}
	}
	return Results;
}

/** Return the first attack matching a Hand (Left, Right, Both) */
FPGAS_AttackType UPGAS_CombatCoreComponent::GetFirstAttackByHand(EPGAS_WeaponHand InHand) const
{
	const FPGAS_AttackType* Found = AttackTypes.FindByPredicate(
		[ & ] (const FPGAS_AttackType& Attack)
		{
			return Attack.Hand == InHand;
		});

	return Found ? *Found : FPGAS_AttackType();
}

/** Return all attacks matching a Hand */
TArray<FPGAS_AttackType> UPGAS_CombatCoreComponent::GetAllAttacksByHand(EPGAS_WeaponHand InHand) const
{
	TArray<FPGAS_AttackType> Results;
	for (const FPGAS_AttackType& Attack : AttackTypes)
	{
		if (Attack.Hand == InHand)
		{
			Results.Add(Attack);
		}
	}
	return Results;
}

/** Return the first attack matching both an AttackType and Hand */
FPGAS_AttackType UPGAS_CombatCoreComponent::GetAttackByTypeAndHand(EPGAS_WeaponAttackType InType, EPGAS_WeaponHand InHand) const
{
	const FPGAS_AttackType* Found = AttackTypes.FindByPredicate(
		[ & ] (const FPGAS_AttackType& Attack)
		{
			return Attack.AttackType == InType && Attack.Hand == InHand;
		});

	return Found ? *Found : FPGAS_AttackType();
}

/** Return the attack that matches a specific GameplayTag */
FPGAS_AttackType UPGAS_CombatCoreComponent::GetAttackByTag(const FGameplayTag& InTag) const
{
	const FPGAS_AttackType* Found = AttackTypes.FindByPredicate(
		[ & ] (const FPGAS_AttackType& Attack)
		{
			return Attack.AbilityTag == InTag;
		});

	return Found ? *Found : FPGAS_AttackType();
}

/**
 * Return the attack matching both AttackType and Index in that type
 * (e.g., Light attack type with Index 0 returns the first light attack, Index 1 the second, etc.)
 * @param InType The type of attack (Light, Medium, Heavy, etc.)
 * @param InIndex The index of the attack within that type (0-based)
 * @return The matching FPGAS_AttackType, or a default instance if not found
 */
FPGAS_AttackType UPGAS_CombatCoreComponent::GetAttackByTypeAndIndex(EPGAS_WeaponAttackType InType, int32 InIndex) const
{
	const TArray<FPGAS_AttackType> AttacksOfType = GetAllAttacksByType(InType);
	if (AttacksOfType.IsValidIndex(InIndex))
	{
		return AttacksOfType[InIndex];
	}
	return FPGAS_AttackType();
}

UPGAS_WeaponDataAsset* UPGAS_CombatCoreComponent::GetEquippedWeaponData() const
{
	// Case 1: If we have a "last known attack", trust its weapon data
	if (LastKnownAttack.WeaponData)
	{
		return LastKnownAttack.WeaponData;
	}

	// Case 2: Try to find any attack that has a weapon
	for (const FPGAS_AttackType& Attack : AttackTypes)
	{
		if (Attack.WeaponData)
		{
			return Attack.WeaponData;
		}
	}

	// Case 3: Nothing equipped
	return nullptr;
}

// ---------------------------
// Combo Functions
// ---------------------------

FPGAS_AttackType UPGAS_CombatCoreComponent::GetNextComboAttack(EPGAS_WeaponAttackType InType, int32 CurrentIndex) const
{
    const FPGAS_AttackType* Found = AttackTypes.FindByPredicate(
        [&](const FPGAS_AttackType& Attack)
        {
            return Attack.AttackType == InType &&
                   Attack.ComboIndex == CurrentIndex;
        });

    return Found ? *Found : FPGAS_AttackType();
}

void UPGAS_CombatCoreComponent::ResetCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Reset"));
	CurrentComboIndex = 0;
	bComboWindowOpen = false;
	bInputBuffered = false;
	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);
}
