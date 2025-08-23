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
 * Base Gameplay Ability class for all abilities in the Paragon GAS project.
 */

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include <Characters/Base/PGAS_CharacterBase.h>
#include "PGAS_GameplayAbilityBase.generated.h"


 /**
 * Base class for all Gameplay Abilities in the Paragon GAS project.
 */
UCLASS(Blueprintable, Abstract, meta=(DisplayName="GAS Gameplay Ability Base"))
class PARAGONGAS_API UPGAS_GameplayAbilityBase : public UGameplayAbility
{
    GENERATED_BODY()

public:
    /** Constructor */
    UPGAS_GameplayAbilityBase();

    /*
     * Functions
     */
    
     /**
     * Called when the ability is activated.
     * Override this in child classes to implement ability logic.
     */
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    /** Called when the ability ends */
    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

    /**
     * Adds gameplay tags to the character ASC.
     */
    UFUNCTION(BlueprintCallable, Category = "Ability|Owner", meta = (DisplayName = "Add Gameplay Tag"))
    void AddGameplayTag(const FGameplayTag NewTag)
    {
        if(CachedCharacter)
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

    /*
     * Properties
     */

    /**
     * Cached reference to the owning character (valid after OnAvatarSet)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Ability|Owner", meta = (AllowPrivateAccess = "true", DisplayName="Cached Character", Tooltip = "Cached reference to the owning character (valid after OnAvatarSet)", Description = "Cached reference to the owning character (valid after OnAvatarSet)"))
    TObjectPtr<APGAS_CharacterBase> CachedCharacter;

    /**
     * Cached reference to the character movement component (valid after OnAvatarSet)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Ability|Owner", meta = (AllowPrivateAccess = "true", DisplayName = "Cached Character Movement", Tooltip = "Cached reference to the character movement component (valid after OnAvatarSet)", Description = "Cached reference to the character movement component (valid after OnAvatarSet)"))
    TObjectPtr<UCharacterMovementComponent> CachedCharacterMovement;

    /**
     * Cached reference to the character level (valid after OnAvatarSet)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Ability|Owner", meta = (AllowPrivateAccess = "true", DisplayName = "Cached Character Level", Tooltip = "Cached reference to the character level (valid after OnAvatarSet)", Description = "Cached reference to the character level (valid after OnAvatarSet)"))
    float CachedCharacterLevel;

protected:
    /** Called when avatar is assigned (safe place to grab owner) */
    virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
