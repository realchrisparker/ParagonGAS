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
 * Date: 8-20-2025
 * =============================================================================
 * Base Gameplay Ability (Montage-based) for the Paragon GAS project.
 */

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility_Montage.h"
#include <Characters/Player/PGAS_PlayerCharacter.h>
#include "PGAS_GameplayAbility_MontageBase.generated.h"

/**
 * Delegates
 */

// Delegate for when a tag notify begins
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTagNotifyBeginSignature, FGameplayEventData, Data);

// Delegate for when a tag notify ends
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTagNotifyEndSignature, FGameplayEventData, Data);


/**
 * Base Gameplay Ability (Montage-based) for the Paragon GAS project.
 */
UCLASS(Blueprintable, Abstract,
    meta = (DisplayName = "GAS Gameplay Ability (Montage) Base",
        Tooltip = "Base class for montage-based gameplay abilities. EndAbility is automatically called when the montage ends.",
        Description = "Base class for montage-based gameplay abilities. EndAbility is automatically called when the montage ends.")
)
class PARAGONGAS_API UPGAS_GameplayAbility_MontageBase : public UGameplayAbility_Montage
{
    GENERATED_BODY()

public:
    /** Constructor */
    UPGAS_GameplayAbility_MontageBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     * Functions
     */

    /** Activate ability (default behavior is broadcast and rely on montage logic in child classes) */
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    /** Clean shutdown when the ability ends */
    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

    /**
     * Called when the ability is activated.
     * Use this one inside of blueprints as the regular ActivateAbility will no longer work as we have overridden it for C++ purposes.
     * This will allow the blueprint to run the code it needs to activate Effects, Costs, etc. and allow designers to customize from blueprint.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "PGAS|Ability", meta = (DisplayName = "On Ability Activated (Blueprint)"))
    void OnAbilityActivatedBlueprint();

    /**
     * Called when the ability is ended.
     * Use this one inside of blueprints as the regular ActivateAbility will no longer work as we have overridden it for C++ purposes.
     * This will allow the blueprint to run the code it needs to activate Effects, Costs, etc. and allow designers to customize from blueprint.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "PGAS|Ability", meta = (DisplayName = "On Ability Ended (Blueprint)"))
    void OnAbilityEndedBlueprint();

    /**
     * Called when the ability montage sends notify begin.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "PGAS|Ability", meta = (DisplayName = "On Tag Notify Begin (Blueprint)"))
    void OnTagNotifyBeginBlueprint(FGameplayEventData Data);

    /**
     * Called when the ability montage sends notify ended.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "PGAS|Ability", meta = (DisplayName = "On Tag Notify End (Blueprint)"))
    void OnTagNotifyEndBlueprint(FGameplayEventData Data);

    /**
     * Adds gameplay tags to the character ASC.
     */
    UFUNCTION(BlueprintCallable, Category = "Ability|Owner", meta = (DisplayName = "Add Gameplay Tag"))
    void AddGameplayTag(const FGameplayTag NewTag)
    {
        if (CachedCharacter)
        {
            CachedCharacter->AddGameplayTag(NewTag);
        }
    }

    /**
     * Removes gameplay tags from the character ASC.
     */
    UFUNCTION(BlueprintCallable, Category = "Ability|Owner", meta = (DisplayName = "Remove Gameplay Tag"))
    void RemoveGameplayTag(const FGameplayTag NewTag)
    {
        if (CachedCharacter)
        {
            CachedCharacter->RemoveGameplayTag(NewTag);
        }
    }

    /**
     * Makes a float value positive.
     */
    UFUNCTION(BlueprintPure, Category = "Ability|Math",
        meta = (DisplayName = "Make Positive",
            Tooltip = "Makes a float value positive",
            Description = "Makes a float value positive"))
    float MakePositive(float Value) const
    {
        return FMath::Abs(Value);
    }

    /**
     * Sets the montage to play.
     */
    UFUNCTION(BlueprintCallable, Category = "Ability",
        meta = (DisplayName = "Set Montage To Play",
            Tooltip = "Sets the montage to play",
            Description = "Sets the montage to play"))
    void SetMontageToPlay(UAnimMontage* NewMontage)
    {
        MontageToPlay = NewMontage;
    }

    /*
     * Properties
     */

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (AllowPrivateAccess = "true", DisplayName = "Stamina Reduction Effect", Tooltip = "Cached reference to the stamina reduction effect", Description = "Cached reference to the stamina reduction effect"))
    TSubclassOf<UGameplayEffect> StaminaReductionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (AllowPrivateAccess = "true", DisplayName = "Stamina Reduction Effect Tag", Tooltip = "Cached reference to the stamina reduction effect tag", Description = "Cached reference to the stamina reduction effect tag"))
    FGameplayTag StaminaReductionEffectTag = FGameplayTag::RequestGameplayTag(FName("Combat.Stamina.Reduction"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (AllowPrivateAccess = "true", DisplayName = "Stamina Cost", Tooltip = "Cached reference to the stamina cost", Description = "Cached reference to the stamina cost"))
    float StaminaCost = 2.0f;

    /** Event triggered when a tag notify begins */
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Ability|Notify")
    FOnTagNotifyBeginSignature OnTagNotifyBegin;

    /** Event triggered when a tag notify ends */
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Ability|Notify")
    FOnTagNotifyEndSignature OnTagNotifyEnd;

protected:
    /*
     * Functions
     */
    
    /** Called when avatar is assigned (safe place to cache references) */
    virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    
    /*
     * Properties
     */

    /** Cached reference to the owning character (valid after OnAvatarSet) */
    UPROPERTY(BlueprintReadOnly, Category = "Ability|Owner", meta = (AllowPrivateAccess = "true", DisplayName = "Cached Character", Tooltip = "Cached reference to the owning character (valid after OnAvatarSet)", Description = "Cached reference to the owning character (valid after OnAvatarSet)"))
    TObjectPtr<APGAS_CharacterBase> CachedCharacter;

    /** Cached reference to the ASC (valid after OnAvatarSet) */
    UPROPERTY(BlueprintReadOnly, Category = "Ability|Owner", meta = (AllowPrivateAccess = "true", DisplayName = "Cached ASC", Tooltip = "Cached reference to the ASC (valid after OnAvatarSet)", Description = "Cached reference to the ASC (valid after OnAvatarSet)"))
    TObjectPtr<UPGAS_AbilitySystemComponent> CachedASC;

    /** Cached reference to the Mesh AnimInstance (valid after OnAvatarSet) */
    UPROPERTY(BlueprintReadOnly, Category = "Ability|Owner", meta = (AllowPrivateAccess = "true", DisplayName = "Cached Anim Instance", Tooltip = "Cached reference to the Mesh AnimInstance (valid after OnAvatarSet)", Description = "Cached reference to the Mesh AnimInstance (valid after OnAvatarSet)"))
    TObjectPtr<UAnimInstance> CachedAnimInstance;

    UPROPERTY(BlueprintReadOnly, Category = "Ability|Owner", meta = (AllowPrivateAccess = "true", DisplayName = "Cached Character Level", Tooltip = "Cached reference to the character level (valid after OnAvatarSet)", Description = "Cached reference to the character level (valid after OnAvatarSet)"))
    float CachedCharacterLevel;

private:
    /*
     * Functions
     */

    /** Callbacks from montage task */
    UFUNCTION()
    void OnMontageCompleted();

    UFUNCTION()
    void OnMontageCancelled();

    UFUNCTION()
    void HandleTagNotifyBegin(FGameplayEventData Payload);

    UFUNCTION()
    void HandleTagNotifyEnd(FGameplayEventData Payload);
};
