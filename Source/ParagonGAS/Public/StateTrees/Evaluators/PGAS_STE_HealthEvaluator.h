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
 * Date: 7-31-2025
 * =============================================================================
 * PGAS_HealthEvaluator.h
 * Evaluator that monitors the Context Actor's health attribute and exposes it to the State Tree as CurrentHealth.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "Components/StateTreeAIComponent.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_STE_HealthEvaluator.generated.h"

UCLASS(Blueprintable,
    meta = (
        DisplayName = "Health Evaluator",
        category = "PGAS",
        ShortTooltip = "Evaluates the health of the context actor.",
        Tooltip = "Monitors the health GAS attribute of the context actor.",
        Icon = "Check_Circle")
)
class PARAGONGAS_API UPGAS_STE_HealthEvaluator : public UStateTreeEvaluatorBlueprintBase
{
    GENERATED_BODY()

public:
    /*
     * Functions
     */

    /** Cache the context actor and reset health. */
    virtual void TreeStart(FStateTreeExecutionContext& Context) override;

    /** Sample health each tick and write to CurrentHealth. */
    virtual void Tick(FStateTreeExecutionContext& Context, float DeltaTime) override;

    /** Clear cached pointers. */
    virtual void TreeStop(FStateTreeExecutionContext& Context) override;

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
        EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const {
        return NSLOCTEXT(
            "PGAS_STEHealth",
            "HealthEvaluatorDesc",
            "Monitors the Context Actor's health"
        );
    };

    virtual FName GetIconName() const {
        return FName("Symbols.Check");
    };

    virtual FColor GetIconColor() const {
        return FColor::Red;
    };

#endif
    
    /*
     * Properties
    */

    /** The owning Enemy Character (set at TreeStart) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<APGAS_EnemyCharacter> Character;

    /** Pointer to the bound controller */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta = (AllowPrivateAccess = "true"))
    TWeakObjectPtr<APGAS_EnemyAIController> AIController;

    /** Out: the actor’s current health. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Output)
    float CurrentHealth = 0.f;
};
