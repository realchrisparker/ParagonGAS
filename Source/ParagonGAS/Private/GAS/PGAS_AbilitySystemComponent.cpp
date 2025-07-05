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

#include "GAS/PGAS_AbilitySystemComponent.h"

/** Constructor */
UPGAS_AbilitySystemComponent::UPGAS_AbilitySystemComponent()
{
    // Initialize
    LastActivatedAbility = nullptr;
    // CurrentComboStep = EPGASComboStep::None;

    // Bind ability events
    AbilityActivatedCallbacks.AddUObject(this, &UPGAS_AbilitySystemComponent::HandleAbilityActivated);
    AbilityEndedCallbacks.AddUObject(this, &UPGAS_AbilitySystemComponent::HandleAbilityEnded);
}

void UPGAS_AbilitySystemComponent::HandleAbilityActivated(UGameplayAbility* ActivatedAbility)
{
    LastActivatedAbility = ActivatedAbility;
}

void UPGAS_AbilitySystemComponent::HandleAbilityEnded(UGameplayAbility* EndedAbility)
{
    if (LastActivatedAbility == EndedAbility)
    {
        LastActivatedAbility = nullptr;
    }
}