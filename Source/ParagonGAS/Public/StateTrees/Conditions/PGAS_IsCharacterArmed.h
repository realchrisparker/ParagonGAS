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
 * Date: 8-10-2025
 * =============================================================================
 * UPGAS_IsCharacterArmed
 * StateTree Condition: Checks if the context actor is armed.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "GameplayTagContainer.h"
#include "PGAS_IsCharacterArmed.generated.h"


UCLASS(Blueprintable, BlueprintType, meta = (DisplayName = "Is Character Armed"), Category = "PGAS")
class PARAGONGAS_API UPGAS_IsCharacterArmed : public UStateTreeConditionBlueprintBase
{
    GENERATED_BODY()

public:
    /*
     * Functions
    */

    /**
     * Tests the condition based on the specified comparison type.
     * @param Context The execution context for the StateTree.
     * @return True if the condition is met, false otherwise.
    */
    virtual bool TestCondition(FStateTreeExecutionContext& Context) const override
    {
        if (Character)
        {
            return Invert ? !Character->IsWeaponEquipped() : Character->IsWeaponEquipped();
        }

        return false;
    }

    /*
     * Properties
    */

    /** The owning Enemy Character (set at TreeStart) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<APGAS_EnemyCharacter> Character;

    /** Pointer to the bound controller */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta = (AllowPrivateAccess = "true"))
    TWeakObjectPtr<APGAS_EnemyAIController> AIController;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition", meta = (DisplayName = "Invert Condition"))
    bool Invert = false;

#if WITH_EDITOR
    /**
     * Gets the description of the condition for editor display.
     * @param ID The unique identifier for the node.
     * @param InstanceDataView The data view for the instance.
     * @param BindingLookup The binding lookup for the state tree.
     * @param Formatting The formatting options for the description.
     * @return The formatted description text.
    */
    virtual FText GetDescription(
        const FGuid& ID,
        FStateTreeDataView InstanceDataView,
        const IStateTreeBindingLookup& BindingLookup,
        EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override
    {
        return NSLOCTEXT("PGAS_IsCharacterArmed", "IsCharacterArmedDesc", "Checks if the context Actor is armed.");
    }
#endif
};
