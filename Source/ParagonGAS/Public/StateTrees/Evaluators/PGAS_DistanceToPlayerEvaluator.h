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
 * PGAS_DistanceToPlayerEvaluator.h
 * Evaluator to calculate and expose the distance from the Context Actor to the Player.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "Components/StateTreeAIComponent.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_DistanceToPlayerEvaluator.generated.h"

/**
 * Evaluator that monitors the distance from the Context Actor to the Player Pawn and exposes it for binding in the State Tree.
*/
UCLASS(Blueprintable,
    meta = (DisplayName = "Distance To Player Evaluator",
        category = "PGAS",
        ShortTooltip = "Evaluates the distance to the player.",
        Tooltip = "Monitors the distance from the context actor to the player.",
        Icon = "Check_Circle"))
class PARAGONGAS_API UPGAS_DistanceToPlayerEvaluator : public UStateTreeEvaluatorBlueprintBase
{
    GENERATED_BODY()

public:
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
            "Calculates distance between the Context Actor and the Player"
        );
    };

    virtual FName GetIconName() const {
        return FName("Symbols.Check");
    };

#endif

    /** The owning Enemy Character (set at TreeStart) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<APGAS_EnemyCharacter> Character;

    /** Pointer to the bound controller */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta = (AllowPrivateAccess = "true"))
    TWeakObjectPtr<APGAS_EnemyAIController> AIController;

    /** Distance from ContextActor to the Player Pawn */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Output, meta = (AllowPrivateAccess = "true"))
    float DistanceToPlayer = 0.f;
};