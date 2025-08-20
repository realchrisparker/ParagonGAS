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
 * Combat Core Component (Dual-Weapon Ready):
 * - Orchestrates combat windows & state via Gameplay Tags (GAS).
 * - Supports per-hand (Left/Right/Both) windows & hitbox routing.
 * - Delegates have handed variants; legacy delegates remain for backward compat.
 * - Abilities still do the gameplay work; the core coordinates & signals.
 */
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Enums/PGAS_WeaponHand.h"
#include "PGAS_CombatCoreComponent.generated.h"

class UAbilitySystemComponent;
class APGAS_CharacterBase;


/** Generic window tag events (legacy, single-channel). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPGAS_CombatWindowTagEvent, FGameplayTag, WindowTag);

/** Handed window tag events (preferred for dual-wield). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPGAS_CombatWindowTagHandedEvent, FGameplayTag, WindowTag, EPGAS_WeaponHand, Hand);

/** Simple start/stop notifications (legacy). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPGAS_CombatSimpleEvent);

/** Handed hitbox start/stop notifications. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPGAS_CombatHitboxHandedEvent, EPGAS_WeaponHand, Hand);

/**
 * The central coordinator for combat timing & state.
 * - Adds/removes loose gameplay tags on the owner ASC to mark windows.
 * - Broadcasts delegates so auxiliary components can react (Hitbox start/stop, UI, etc.).
 * - Provides helpers to activate/cancel abilities by tag; can stamp a hand-context tag briefly.
 *
 * NOTE: Actual attacks/defense live in Gameplay Abilities & Effects.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (PGAS), meta = (BlueprintSpawnableComponent, DisplayName = "PGAS Combat Core Component"))
class PARAGONGAS_API UPGAS_CombatCoreComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPGAS_CombatCoreComponent();

    // ---------------------------
    // Query
    // ---------------------------
    UFUNCTION(BlueprintPure, Category = "PGAS|Combat|Tags")
    bool HasCombatTag(const FGameplayTag& Tag) const;

    // ---------------------------
    // Window control (hand-aware)
    // ---------------------------

    /** Open a specific window tag for a given hand (or Both). Duration<=0 keeps it open until closed. */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Tags")
    void OpenWindowTagForHand(const FGameplayTag& Tag, EPGAS_WeaponHand Hand, float Duration = 0.f);

    /** Close a specific window tag for a given hand (or Both). */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Tags")
    void CloseWindowTagForHand(const FGameplayTag& Tag, EPGAS_WeaponHand Hand);

    /** Legacy helpers (single-channel). */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Tags", meta = (DisplayName = "Open Window Tag (Legacy)"))
    void OpenWindowTag(const FGameplayTag& Tag, float Duration = 0.f) { OpenWindowTagForHand(Tag, EPGAS_WeaponHand::Both, Duration); }

    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Tags", meta = (DisplayName = "Close Window Tag (Legacy)"))
    void CloseWindowTag(const FGameplayTag& Tag) { CloseWindowTagForHand(Tag, EPGAS_WeaponHand::Both); }

    // ---------------------------
    // Ability helpers
    // ---------------------------

    /**
     * Activate an ability by tag. Optionally stamps a brief hand-context tag
     * (e.g., Combat.Hand.Left/Right) so the ability can branch on the active hand.
     */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Abilities")
    bool ActivateAbilityByTag(const FGameplayTag& AbilityTag,
        EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both,
        bool bAllowRemoteActivation = true,
        bool bSetHandContextTag = true,
        float ContextTagDuration = -1.f);

    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Abilities")
    void CancelAbilitiesByTag(const FGameplayTag& AbilityTag);

    // ---------------------------
    // Notify-friendly helpers (handed)
    // ---------------------------

    // Attack Active window
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Windows")
    void Notify_AttackWindowBegin(EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both, float DurationSec = 0.f);
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Windows")
    void Notify_AttackWindowEnd(EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both);

    // Attack CanChain window
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Windows")
    void Notify_CanChainBegin(EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both, float DurationSec = 0.f);
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Windows")
    void Notify_CanChainEnd(EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both);

    // Parry window
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Windows")
    void Notify_ParryWindowBegin(EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both, float DurationSec = 0.f);
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Windows")
    void Notify_ParryWindowEnd(EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both);

    // Dodge i-frames (usually global; hand kept for parity)
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Windows")
    void Notify_DodgeIFramesBegin(EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both, float DurationSec = 0.f);
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Windows")
    void Notify_DodgeIFramesEnd(EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both);

    // Hitbox start/stop (handed + legacy)
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Hitbox")
    void Notify_HitboxStart(EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both);
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Hitbox")
    void Notify_HitboxStop(EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Both);

    // ---------------------------
    // Events
    // ---------------------------

    // Legacy (single-channel)
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Events")
    FPGAS_CombatWindowTagEvent OnWindowStarted;
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Events")
    FPGAS_CombatWindowTagEvent OnWindowEnded;
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Events")
    FPGAS_CombatSimpleEvent OnHitboxStartLegacy;
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Events")
    FPGAS_CombatSimpleEvent OnHitboxStopLegacy;

    // Preferred (hand-aware)
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Events")
    FPGAS_CombatWindowTagHandedEvent OnWindowStartedHanded;
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Events")
    FPGAS_CombatWindowTagHandedEvent OnWindowEndedHanded;

    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Events")
    FPGAS_CombatHitboxHandedEvent OnHitboxStartHanded;
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Events")
    FPGAS_CombatHitboxHandedEvent OnHitboxStopHanded;

    // ---------------------------
    // Debug
    // ---------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Combat|Debug")
    bool bDebugWindows = false;

    // ---------------------------
    // Configurable Tags (generic + per-hand overrides)
    // ---------------------------

    /** Generic tags */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag AttackActiveTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag AttackCanChainTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag ParryWindowTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag DodgeIFrameTag;

    /** Per-hand overrides (if set, take precedence over generic) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag AttackActive_LeftTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag AttackActive_RightTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag AttackCanChain_LeftTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag AttackCanChain_RightTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag ParryWindow_LeftTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag ParryWindow_RightTag;

    /** Optional: hand-context tags to stamp briefly when activating abilities */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag HandContext_LeftTag;  // e.g., Combat.Hand.Left
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Tags|Config", meta = (Categories = "Gameplay"))
    FGameplayTag HandContext_RightTag; // e.g., Combat.Hand.Right

    /** Default duration to keep the hand-context tag after activation (<=0 uses one frame/tick). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Combat|Config")
    float DefaultHandContextDuration = 0.2f;

    // ---------------------------
    // Optional weapon profiles (placeholders you can replace with your own asset class)
    // ---------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Combat|Weapon")
    TObjectPtr<UObject> LeftWeaponProfile = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Combat|Weapon")
    TObjectPtr<UObject> RightWeaponProfile = nullptr;

protected:
    virtual void BeginPlay() override;

    /** Cached owner pointers */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Owner", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<APGAS_CharacterBase> CachedCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Owner", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAbilitySystemComponent> CachedASC;

private:
    // Internal helpers
    void AddWindowTag_Internal(const FGameplayTag& Tag, float Duration, EPGAS_WeaponHand Hand);
    void RemoveWindowTag_Internal(const FGameplayTag& Tag, EPGAS_WeaponHand Hand);

    /** Resolve per-hand override; falls back to Generic if override not set. */
    FGameplayTag ResolveHandTag(const FGameplayTag& Generic, const FGameplayTag& Left, const FGameplayTag& Right, EPGAS_WeaponHand Hand) const;

    /** Unique-collect resolved tags for Both so we don't double-add same tag. */
    void ResolveAndApplyForHand(const FGameplayTag& Generic, const FGameplayTag& Left, const FGameplayTag& Right,
        EPGAS_WeaponHand Hand, float Duration, bool bOpen);

    /** Timer handles per concrete tag (left/right tags are different FGameplayTags). */
    TMap<FGameplayTag, FTimerHandle> WindowTimers;

    /** Debug print helper. */
    void DebugPrint(const FString& Msg) const;
};
