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
#include <Data/Assets/PGAS_WeaponDataAsset.h>
#include "PGAS_CombatCoreComponent.generated.h"

 /*
  * Forward Declarations
 */

class UAbilitySystemComponent;
class APGAS_CharacterBase;

/*
 * Structs
*/

USTRUCT(BlueprintType, meta = (DisplayName = "PGAS Attack Type", Description = "Represents an attack type in the PGAS Combat System"))
struct PARAGONGAS_API FPGAS_AttackType
{
    GENERATED_BODY()

public:

    // The gameplay tag that identifies the ability or attack
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Combat", meta = (DisplayName = "Ability Tag", Description = "The gameplay tag that identifies the ability or attack"))
    FGameplayTag AbilityTag;

    // Which hand performs this attack (left/right/both)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Combat", meta = (DisplayName = "Attack Hand", Description = "Which hand performs this attack (left/right/both)"))
    EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Right;

    // The weapon data asset associated with this attack
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Combat", meta = (DisplayName = "Weapon Data", Description = "The weapon data asset associated with this attack"))
    TObjectPtr<UPGAS_WeaponDataAsset> WeaponData = nullptr;

    // The type of attack (light, medium, heavy)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Combat", meta = (DisplayName = "Attack Type", Description = "The type of attack (light, medium, heavy, etc.)"))
    EPGAS_WeaponAttackType AttackType = EPGAS_WeaponAttackType::Light;
};


/*
 * Delegates
*/

// Delegate for combat windows
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FPGAS_CombatWindowEventSignature,
    FPGAS_AttackType, AttackData
);


/**
 * The central coordinator for combat timing & state.
 * - Adds/removes loose gameplay tags on the owner ASC to mark windows.
 * - Broadcasts delegates so auxiliary components can react (Hitbox start/stop, UI, etc.).
 * - Provides helpers to activate/cancel abilities by tag; can stamp a hand-context tag briefly.
 *
 * NOTE: Actual attacks/defense live in Gameplay Abilities & Effects.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (PGAS), 
    meta = (BlueprintSpawnableComponent, 
        DisplayName = "PGAS Combat Core Component", 
        Description = "Central coordinator for combat timing & state.", 
        Tooltip = "Handles combat windows and state management.")
)
class PARAGONGAS_API UPGAS_CombatCoreComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Constructor
    UPGAS_CombatCoreComponent();

    /*
     * Properties
    */

    /**
     * All available attacks for this character.
     * Designers can add/remove entries in the editor.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Attacks")
    TArray<FPGAS_AttackType> AttackTypes;

    // ---------------------------
    // Windowing
    // ---------------------------

    /** Delegate broadcast when a combat window is opened or closed */
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Events")
    FPGAS_CombatWindowEventSignature OnCombatWindowOpen;

    /** Delegate broadcast when a combat window is closed */
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Events")
    FPGAS_CombatWindowEventSignature OnCombatWindowClose;

    /*
     * Functions
    */

    // ---------------------------
    // Ability helpers
    // ---------------------------

    /**
     * Activate an ability by tag.
     * @param AttackData The attack data to use for activation.
     */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Abilities")
    bool ActivateAbilityByTag(const FGameplayTag& AbilityTag);

    /**
     * Cancel abilities by tag.
     * @param AttackData The combat tag type to cancel abilities for.
     */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Abilities")
    void CancelAbilityByTag(const FGameplayTag& AbilityTag);

    // ---------------------------
    // Attack Lookup Helpers
    // ---------------------------

    /** Return the first attack matching an AttackType (Light, Heavy, etc.) */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Helpers")
    FPGAS_AttackType GetFirstAttackByType(EPGAS_WeaponAttackType InType) const;

    /** Return all attacks matching an AttackType */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Helpers")
    TArray<FPGAS_AttackType> GetAllAttacksByType(EPGAS_WeaponAttackType InType) const;

    /** Return the first attack matching a Hand (Left, Right, Both) */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Helpers")
    FPGAS_AttackType GetFirstAttackByHand(EPGAS_WeaponHand InHand) const;

    /** Return all attacks matching a Hand */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Helpers")
    TArray<FPGAS_AttackType> GetAllAttacksByHand(EPGAS_WeaponHand InHand) const;

    /** Return the attack that matches a specific GameplayTag */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Helpers")
    FPGAS_AttackType GetAttackByTag(const FGameplayTag& InTag) const;

    /** Return the first attack matching both AttackType and Hand */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Helpers")
    FPGAS_AttackType GetAttackByTypeAndHand(EPGAS_WeaponAttackType InType, EPGAS_WeaponHand InHand) const;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

    /*
     * Properties
    */

    // ---------------------------
    // Cached owner pointers
    // ---------------------------

    TObjectPtr<APGAS_CharacterBase> CachedCharacter; // Cached reference to the owning character
    TObjectPtr<UAbilitySystemComponent> CachedASC; // Cached reference to the ability system component

    FPGAS_AttackType LastKnownAttack; // Last known attack
    TArray<FDelegateHandle> RegisteredTagHandles; // Array to hold registered tag handles

    /*
     * Functions
    */

    /** Resolve a GameplayTag from the given combat tag enum */
    // FGameplayTag ResolveTagFromEnum(EPGAS_CombatTagType CombatTag) const;

    // /** Resolve a CombatTag enum type from a GameplayTag property */
    // EPGAS_CombatTagType ResolveEnumFromTag(const FGameplayTag& Tag) const;

    // /** Set the last known attack hand from the given combat tag enum */
    // void SetLastKnownAttackHandFromEnum(EPGAS_CombatTagType CombatTag);

    /** Handle changes to the attack window tag */
    void HandleAttackWindowTagChanged(const FGameplayTag Tag, int32 NewCount);
};
