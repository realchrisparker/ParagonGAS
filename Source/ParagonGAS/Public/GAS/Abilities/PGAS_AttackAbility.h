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
 * Date: 7-30-2025
 * =============================================================================
 * PGAS_AttackAbility.h
 * Minimal gameplay ability for basic attack actions.
 */

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Animation/AnimMontage.h"
#include "GameplayTagContainer.h"
#include "PGAS_AttackAbility.generated.h"


 /*
  * Delegates
 */

 /**
  * Delegate for Montage State Notify events.
  * This delegate is used to notify when a montage state notify occurs.
  * @param NotifyTag The tag associated with the notify.
  * @param EventData The event data associated with the notify.
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMontageStateNotify, FGameplayTag /*NotifyTag*/, FGameplayEventData /*EventData*/);


/*
 * PGAS_AttackAbility
 * A Gameplay Ability class with support for Animation Montages. Allows blueprint designers and C++ to assign a Montage asset.
*/
UCLASS(Blueprintable, meta=(ShortTooltip="Attack ability"))
class PARAGONGAS_API UPGAS_AttackAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    /*
     * Functions
    */

    // Constructor
    UPGAS_AttackAbility();

    /** Called when the ability is activated */
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;

    /** Called when the ability ends */
    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled
    ) override;

    /*
     * Properties
    */

    /** GameplayTag this ability listens for when trying to activate by tag */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (displayName = "Activation Tag",
        ToolTip = "The GameplayTag that this ability listens for when trying to activate. This allows the ability to be activated by a specific tag."))
    FGameplayTag ActivationTag;

    /** The montage asset to play when this ability activates. Blueprint assignable. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability",
        meta = (DisplayName = "Montage To Play", ToolTip = "This allows for the ability to play a specific animation montage when activated."))
    TObjectPtr<UAnimMontage> MontageToPlay;

    /**
     * Tags that this ability will own while active.
     * Wire this into ActivationOwnedTags in your constructor:
     *   ActivationOwnedTags = OwnedActivationTags;
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (Categories = "GameplayAbility"))
    FGameplayTagContainer OwnedActivationTags;


    /** Broadcast when a Montage State Notify occurs */
    static FOnMontageStateNotify OnMontageStateNotify;

private:
    /*
    * Properties
    */

    // Cached properties to use during ability activation and other functions.
    FGameplayAbilitySpecHandle CachedHandle;
    const FGameplayAbilityActorInfo* CachedActorInfo = nullptr;
    FGameplayAbilityActivationInfo CachedActivationInfo;
    const FGameplayEventData* CachedTriggerEventData = nullptr;
};
