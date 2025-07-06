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

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include <Enums/PGAS_ComboStep.h>
#include "PGAS_AbilitySystemComponent.generated.h"

/**
 *
*/
UCLASS()
class PARAGONGAS_API UPGAS_AbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    /*
    * Functions
    */

    /** Constructor */
    UPGAS_AbilitySystemComponent();

    /*
    * Properties
    */

    /** The current step in the combo sequence */
    UPROPERTY(BlueprintReadWrite, Category = "Combo")
    EPGASComboStep CurrentComboStep;

    /** The last activated ability */
    UPROPERTY(BlueprintReadOnly, Category = "Abilities", meta=(DisplayName = "Last Activated Ability"))
    UGameplayAbility* LastActivatedAbility;

private:
    /** Internal handler for ability activation */
    void HandleAbilityActivated(UGameplayAbility* ActivatedAbility);

    /** Internal handler for ability end */
    void HandleAbilityEnded(UGameplayAbility* EndedAbility);
};
