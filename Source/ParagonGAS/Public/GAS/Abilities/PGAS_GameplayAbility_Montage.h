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
 * Date: 6-29-2025
 * =============================================================================
 * PGAS_GameplayAbility_Montage
 * A base Gameplay Ability class with support for Animation Montages. Allows blueprint designers and C++ to assign a Montage asset to any inheriting Ability.
 */

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Animation/AnimMontage.h"
#include <Enums/PGAS_ComboStep.h>
#include "PGAS_GameplayAbility_Montage.generated.h"


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
 * PGAS_GameplayAbility_Montage
 * A base Gameplay Ability class with support for Animation Montages. Allows blueprint designers and C++ to assign a Montage asset to any inheriting Ability.
*/
UCLASS(Abstract, Blueprintable)
class PARAGONGAS_API UPGAS_GameplayAbility_Montage : public UGameplayAbility
{
    GENERATED_BODY()

public:
    /*
    * Functions
    */

    // Constructor
    UPGAS_GameplayAbility_Montage();

    /** Delegate for montage state notifications. */
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    /** End the ability */
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, 
        const FGameplayAbilityActorInfo* ActorInfo, 
        const FGameplayAbilityActivationInfo ActivationInfo, 
        bool bReplicateEndAbility, 
        bool bWasCancelled);

    /**
     * Event that broadcasts when a montage state notify occurs. (For Blueprints)
     * This is used to add or remove gameplay tags during montage states.
     * @param AddGameplayTags The gameplay tags to add when the notify begins.
     * @param RemoveGameplayTags The gameplay tags to remove when the notify ends.
    */
    UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Montage|Notify", meta = (DisplayName = "On Montage State Notify", 
        ToolTip = "This is called when a montage state notify occurs. This allows for custom logic to be executed when a specific state notify is triggered."))
    void MontageStateNotify(FGameplayTag NotifyTag, FGameplayEventData EventData);

    /**
     * Event that broadcasts when a montage notify occurs. (For Blueprints)
     * This is a generic notify that can be used for any notify in the montage.
     * @param NotifyName The name of the notify that was triggered.
    */
    UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Montage|Notify", meta = (DisplayName = "On Montage Notify", 
        ToolTip = "This is called when a montage notify occurs. This allows for custom logic to be executed when a specific notify is triggered."))
    void MontageNotify(FName NotifyName);

    /**
     * This is called when the montage completes.
    */
    UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Montage", meta = (DisplayName = "On Montage Completed", 
        ToolTip = "This is called when the montage completes. This allows for cleanup or resetting state when the montage is no longer playing."))
    void MontageCompleted();

    /**
     * This is called when the montage is interrupted.
    */
    UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Montage", meta = (DisplayName = "On Montage Interrupted", 
        ToolTip = "This is called when the montage is interrupted. This allows for cleanup or resetting state when the montage is no longer playing."))
    void MontageInterrupted();

    /**
     * This is called when the montage is cancelled.
    */
    UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Montage", meta = (DisplayName = "On Montage Cancelled", 
        ToolTip = "This is called when the montage is cancelled. This allows for cleanup or resetting state when the montage is no longer playing."))
    void MontageCancelled();

    /**
     * Attempts to activate the next ability in the combo sequence.
     * This allows for chaining abilities together in a combo.
     * If the next ability is not set, it will not do anything.
     * This is called when the player presses the input key for the next combo ability.
     * It will check if the combo window is open and if so, it will activate the next ability.
    */
    UFUNCTION(BlueprintCallable, Category = "Ability|Montage", meta = (DisplayName = "Try Activate Next Combo",
        ToolTip = "Attempts to activate the next ability in the combo sequence. This allows for chaining abilities together in a combo."))
    void TryActivateNextCombo();

    /*
    * Properties
    */

    /** The montage asset to play when this ability activates. Blueprint assignable. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Montage", meta = (DisplayName = "Montage To Play", 
        ToolTip = "The animation montage to play when this ability is activated. This allows for the ability to play a specific animation montage when activated."))
    TObjectPtr<UAnimMontage> MontageToPlay;

    /** The combo step associated with this ability. (Default is none, meaning it isn't a part of any combo)*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Combo", meta = (DisplayName = "Combo Step",
        ToolTip = "The step in the combo sequence this ability represents. Used to track combo progression."))
    EPGASComboStep ComboStep = EPGASComboStep::None;

    // Reference to the next combo ability class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Combo", meta = (DisplayName = "Next Combo Ability",
        ToolTip = "The next ability to activate in the combo sequence. This allows for chaining abilities together in a combo."))
    TSubclassOf<UPGAS_GameplayAbility_Montage> NextComboAbility;

    // Tracks if combo window is open
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability|Combo", meta = (DisplayName = "Is Combo Window Open",
        ToolTip = "Tracks if the combo window is currently open, allowing for combo chaining."))
    bool bComboWindowOpen = false;

    /** Broadcast when a Montage State Notify occurs */
    static FOnMontageStateNotify OnMontageStateNotify;

protected:

    UFUNCTION(BlueprintCallable, Category = "Ability|Combo")
    void OnComboInputReceived();

private:
    /*
    * Functions
    */
    
    UFUNCTION()
    void OnMontageCompleted();

    UFUNCTION()
    void OnMontageInterrupted();

    UFUNCTION()
    void OnMontageCancelled();

    UFUNCTION()
    void OnNotifyBegin(FGameplayEventData Payload);

    UFUNCTION()
    void OnNotifyEnd(FGameplayEventData Payload);

    /*
    * Properties
    */

    // Cached properties to use during ability activation and other functions.
    FGameplayAbilitySpecHandle CachedHandle;
    const FGameplayAbilityActorInfo* CachedActorInfo = nullptr;
    FGameplayAbilityActivationInfo CachedActivationInfo;
    const FGameplayEventData* CachedTriggerEventData = nullptr;
};
 