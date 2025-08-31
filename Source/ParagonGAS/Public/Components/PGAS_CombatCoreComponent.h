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
#include <Structs/PGAS_AttackType.h>
#include "PGAS_CombatCoreComponent.generated.h"

 /*
  * Forward Declarations
 */

class UAbilitySystemComponent;
class APGAS_CharacterBase;
class UPGAS_WeaponDataAsset;


/*
 * Delegates
*/

// Delegate for combat windows
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPGAS_CombatWindowEventSignature, FPGAS_AttackType, AttackData);


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

    /** Returns the equipped weapon data asset.
     *  If dual wielding, returns the last known attack's weapon data.
     *  If none, returns nullptr.
     */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Helpers")
    UPGAS_WeaponDataAsset* GetEquippedWeaponData() const;

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
    
    /** Handle changes to the attack window tag */
    void HandleAttackWindowTagChanged(const FGameplayTag Tag, int32 NewCount);
};
