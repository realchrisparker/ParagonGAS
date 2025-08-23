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

#include "Components/PGAS_CombatCoreComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Characters/Base/PGAS_CharacterBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

UPGAS_CombatCoreComponent::UPGAS_CombatCoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPGAS_CombatCoreComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<APGAS_CharacterBase>(GetOwner()); // Cache the character owner
	if (CachedCharacter)
    {
        CachedASC = CachedCharacter->GetAbilitySystemComponent(); // Cache the Ability System Component from the character
	}

#if WITH_EDITOR
	if (!CachedCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatCore: Owner is not APGAS_CharacterBase."));
	}
	if (!CachedASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatCore: No AbilitySystemComponent found on owner."));
	}
#endif
}

bool UPGAS_CombatCoreComponent::HasCombatTag(const FGameplayTag& Tag) const
{
	return CachedASC ? CachedASC->HasMatchingGameplayTag(Tag) : false;
}

void UPGAS_CombatCoreComponent::OpenWindowTagForHand(const FGameplayTag& Tag, EPGAS_WeaponHand Hand, float Duration)
{
	// Caller provides a concrete tag. For Both, open once for that tag.
	if (!Tag.IsValid())
	{
		return;
	}

	if (Hand == EPGAS_WeaponHand::Both)
	{
		AddWindowTag_Internal(Tag, Duration, Hand);
	}
	else
	{
		AddWindowTag_Internal(Tag, Duration, Hand);
	}
}

void UPGAS_CombatCoreComponent::CloseWindowTagForHand(const FGameplayTag& Tag, EPGAS_WeaponHand Hand)
{
	if (!Tag.IsValid())
	{
		return;
	}

	if (Hand == EPGAS_WeaponHand::Both)
	{
		RemoveWindowTag_Internal(Tag, Hand);
	}
	else
	{
		RemoveWindowTag_Internal(Tag, Hand);
	}
}

bool UPGAS_CombatCoreComponent::ActivateAbilityByTag(const FGameplayTag& AbilityTag,
	EPGAS_WeaponHand Hand, bool bAllowRemoteActivation, bool bSetHandContextTag, float ContextTagDuration)
{
	if (!CachedCharacter)
	{
		return false;
	}

	// Optionally stamp a brief hand-context tag so the ability can branch.
	FGameplayTag ContextTag;
	if (bSetHandContextTag)
	{
		if (Hand == EPGAS_WeaponHand::Left && HandContext_LeftTag.IsValid())
		{
			ContextTag = HandContext_LeftTag;
		}
		else if (Hand == EPGAS_WeaponHand::Right && HandContext_RightTag.IsValid())
		{
			ContextTag = HandContext_RightTag;
		}

		if (ContextTag.IsValid() && CachedASC)
		{
			// Default duration if caller passed sentinel
			if (ContextTagDuration <= 0.f)
			{
				ContextTagDuration = DefaultHandContextDuration;
			}
			AddWindowTag_Internal(ContextTag, ContextTagDuration, Hand);
		}
	}

	FGameplayTagContainer Tags;
	Tags.AddTag(AbilityTag);
	return CachedCharacter->ActivateAbilitiesWithTags(Tags, bAllowRemoteActivation);
}

void UPGAS_CombatCoreComponent::CancelAbilitiesByTag(const FGameplayTag& AbilityTag)
{
	if (!CachedCharacter)
	{
		return;
	}

	FGameplayTagContainer Tags;
	Tags.AddTag(AbilityTag);
	CachedCharacter->CancelAbilitiesWithTags(Tags);
}

// ---------------------------
// Notify helpers (handed)
// ---------------------------

void UPGAS_CombatCoreComponent::Notify_AttackWindowBegin(EPGAS_WeaponHand Hand, float DurationSec)
{
	ResolveAndApplyForHand(AttackActiveTag, AttackActive_LeftTag, AttackActive_RightTag, Hand, DurationSec, /*bOpen*/true);

	// Hitbox should start for that hand
	OnHitboxStartLegacy.Broadcast();
	OnHitboxStartHanded.Broadcast(Hand);
}

void UPGAS_CombatCoreComponent::Notify_AttackWindowEnd(EPGAS_WeaponHand Hand)
{
	ResolveAndApplyForHand(AttackActiveTag, AttackActive_LeftTag, AttackActive_RightTag, Hand, /*Duration*/0.f, /*bOpen*/false);

	OnHitboxStopLegacy.Broadcast();
	OnHitboxStopHanded.Broadcast(Hand);
}

void UPGAS_CombatCoreComponent::Notify_CanChainBegin(EPGAS_WeaponHand Hand, float DurationSec)
{
	ResolveAndApplyForHand(AttackCanChainTag, AttackCanChain_LeftTag, AttackCanChain_RightTag, Hand, DurationSec, /*bOpen*/true);
}

void UPGAS_CombatCoreComponent::Notify_CanChainEnd(EPGAS_WeaponHand Hand)
{
	ResolveAndApplyForHand(AttackCanChainTag, AttackCanChain_LeftTag, AttackCanChain_RightTag, Hand, /*Duration*/0.f, /*bOpen*/false);
}

void UPGAS_CombatCoreComponent::Notify_ParryWindowBegin(EPGAS_WeaponHand Hand, float DurationSec)
{
	ResolveAndApplyForHand(ParryWindowTag, ParryWindow_LeftTag, ParryWindow_RightTag, Hand, DurationSec, /*bOpen*/true);
}

void UPGAS_CombatCoreComponent::Notify_ParryWindowEnd(EPGAS_WeaponHand Hand)
{
	ResolveAndApplyForHand(ParryWindowTag, ParryWindow_LeftTag, ParryWindow_RightTag, Hand, /*Duration*/0.f, /*bOpen*/false);
}

void UPGAS_CombatCoreComponent::Notify_DodgeIFramesBegin(EPGAS_WeaponHand Hand, float DurationSec)
{
	// Typically global; we still pass Hand for parity
	const FGameplayTag UseTag = ResolveHandTag(DodgeIFrameTag, FGameplayTag(), FGameplayTag(), Hand);
	if (UseTag.IsValid())
	{
		AddWindowTag_Internal(UseTag, DurationSec, Hand);
	}
}

void UPGAS_CombatCoreComponent::Notify_DodgeIFramesEnd(EPGAS_WeaponHand Hand)
{
	const FGameplayTag UseTag = ResolveHandTag(DodgeIFrameTag, FGameplayTag(), FGameplayTag(), Hand);
	if (UseTag.IsValid())
	{
		RemoveWindowTag_Internal(UseTag, Hand);
	}
}

void UPGAS_CombatCoreComponent::Notify_HitboxStart(EPGAS_WeaponHand Hand)
{
	OnHitboxStartLegacy.Broadcast();
	OnHitboxStartHanded.Broadcast(Hand);
}

void UPGAS_CombatCoreComponent::Notify_HitboxStop(EPGAS_WeaponHand Hand)
{
	OnHitboxStopLegacy.Broadcast();
	OnHitboxStopHanded.Broadcast(Hand);
}

void UPGAS_CombatCoreComponent::Notify_BlockWindowBegin(EPGAS_WeaponHand Hand, float DurationSec)
{
    ResolveAndApplyForHand(BlockWindowTag, BlockWindow_LeftTag, BlockWindow_RightTag, Hand, DurationSec, true);
}

void UPGAS_CombatCoreComponent::Notify_BlockWindowEnd(EPGAS_WeaponHand Hand)
{
    ResolveAndApplyForHand(BlockWindowTag, BlockWindow_LeftTag, BlockWindow_RightTag, Hand, 0.f, false);
}

// ---------------------------
// Internals
// ---------------------------

void UPGAS_CombatCoreComponent::AddWindowTag_Internal(const FGameplayTag& Tag, float Duration, EPGAS_WeaponHand Hand)
{
	if (!CachedASC || !Tag.IsValid())
	{
		return;
	}

	// Add loose tag
	const bool bWasPresent = CachedASC->HasMatchingGameplayTag(Tag);
	CachedASC->AddLooseGameplayTag(Tag);

	// Broadcast events
	OnWindowStarted.Broadcast(Tag);
	OnWindowStartedHanded.Broadcast(Tag, Hand);

	if (bDebugWindows)
	{
		DebugPrint(FString::Printf(TEXT("[Window +] %s (%.2fs, Hand=%s)%s"),
			*Tag.ToString(), Duration, *UEnum::GetValueAsString(Hand),
			bWasPresent ? TEXT(" [dup]") : TEXT("")));
	}

	// Duration-based auto-close
	if (Duration > 0.f)
	{
		// Clear any existing timer for this exact tag
		if (FTimerHandle* Existing = WindowTimers.Find(Tag))
		{
			GetWorld()->GetTimerManager().ClearTimer(*Existing);
		}

		FTimerHandle& Handle = WindowTimers.FindOrAdd(Tag);
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateWeakLambda(this, [this, Tag, Hand]()
			{
				RemoveWindowTag_Internal(Tag, Hand);
			}),
			Duration,
			false
		);
	}
}

void UPGAS_CombatCoreComponent::RemoveWindowTag_Internal(const FGameplayTag& Tag, EPGAS_WeaponHand Hand)
{
	if (!CachedASC || !Tag.IsValid())
	{
		return;
	}

	// Stop timer for this exact tag
	if (FTimerHandle* Handle = WindowTimers.Find(Tag))
	{
		GetWorld()->GetTimerManager().ClearTimer(*Handle);
		WindowTimers.Remove(Tag);
	}

	// Remove loose tag if present
	if (CachedASC->HasMatchingGameplayTag(Tag))
	{
		CachedASC->RemoveLooseGameplayTag(Tag);

		OnWindowEnded.Broadcast(Tag);
		OnWindowEndedHanded.Broadcast(Tag, Hand);

		if (bDebugWindows)
		{
			DebugPrint(FString::Printf(TEXT("[Window -] %s (Hand=%s)"),
				*Tag.ToString(), *UEnum::GetValueAsString(Hand)));
		}
	}
}

FGameplayTag UPGAS_CombatCoreComponent::ResolveHandTag(const FGameplayTag& Generic,
                                                       const FGameplayTag& Left,
                                                       const FGameplayTag& Right,
                                                       EPGAS_WeaponHand Hand) const
{
	switch (Hand)
	{
	case EPGAS_WeaponHand::Left:
		return Left.IsValid() ? Left : Generic;
	case EPGAS_WeaponHand::Right:
		return Right.IsValid() ? Right : Generic;
	case EPGAS_WeaponHand::Both:
	default:
		return Generic;
	}
}

void UPGAS_CombatCoreComponent::ResolveAndApplyForHand(const FGameplayTag& Generic,
                                                       const FGameplayTag& Left,
                                                       const FGameplayTag& Right,
                                                       EPGAS_WeaponHand Hand,
                                                       float Duration,
                                                       bool bOpen)
{
	// Collect unique concrete tags we will operate on
	TSet<FGameplayTag> UniqueTags;

	if (Hand == EPGAS_WeaponHand::Both)
	{
		const FGameplayTag L = Left.IsValid() ? Left : Generic;
		const FGameplayTag R = Right.IsValid() ? Right : Generic;

		if (L.IsValid()) { UniqueTags.Add(L); }
		if (R.IsValid()) { UniqueTags.Add(R); }
	}
	else
	{
		const FGameplayTag Use = ResolveHandTag(Generic, Left, Right, Hand);
		if (Use.IsValid())
		{
			UniqueTags.Add(Use);
		}
	}

	for (const FGameplayTag& Tag : UniqueTags)
	{
		if (bOpen)
		{
			AddWindowTag_Internal(Tag, Duration, Hand);
		}
		else
		{
			RemoveWindowTag_Internal(Tag, Hand);
		}
	}
}

void UPGAS_CombatCoreComponent::DebugPrint(const FString& Msg) const
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("CombatCore: %s"), *Msg));
	}
	UE_LOG(LogTemp, Log, TEXT("CombatCore: %s"), *Msg);
#endif
}
