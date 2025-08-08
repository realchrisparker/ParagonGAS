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
 * Date: 8-7-2025
 * =============================================================================
 * PGAS_PlayerEvaluator.h
 * Native StateTree evaluator that monitors the player.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeNodeBase.h"
#include "StateTreeLinker.h"
#include "Kismet/GameplayStatics.h"
#include <Characters/Player/PGAS_PlayerCharacter.h>
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_PlayerEvaluator.generated.h"

/**
 * Evaluator that monitors the player
*/
UCLASS(Blueprintable,
    meta = (DisplayName = "Player Evaluator",
        category = "PGAS",
        ShortTooltip = "Evaluates the player.",
        Tooltip = "Monitors the player.",
        Icon = "Check_Circle")
)
class PARAGONGAS_API UPGAS_PlayerEvaluator : public UStateTreeEvaluatorBlueprintBase
{
    GENERATED_BODY()

public:
    /** Cache the context actor and reset health. */
    virtual void TreeStart(FStateTreeExecutionContext& Context) override
    {
        //
    }

    /** Sample health each tick and write to CurrentHealth. */
    virtual void Tick(FStateTreeExecutionContext& Context, float DeltaTime) override
    {
        UWorld* World = Character->GetWorld();
        if (!World)
        {
            SetDefaultValues(Context);
            return;
        }

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
        if (!PlayerPawn)
        {
            SetDefaultValues(Context);
            return;
        }

        // Calculate the distance to the player
        DistanceToPlayer = FVector::Dist(Character->GetActorLocation(), PlayerPawn->GetActorLocation());

        // Check player information
        if (APGAS_PlayerCharacter* PlayerCharacter = Cast<APGAS_PlayerCharacter>(PlayerPawn))
        {
            PlayerCharacterLevel = PlayerCharacter->GetCharacterLevel(); // Get the player character's level
            IsPlayerAttacking = PlayerCharacter->IsAttacking(); // Check if the player character is attacking
            IsPlayerDead = PlayerCharacter->IsDead(); // Check if the player character is dead
        }
    }

    /** Clear cached pointers. */
    virtual void TreeStop(FStateTreeExecutionContext& Context) override
    {
        SetDefaultValues(Context);
    }

    /**
     * Set default values for the instance data
     * @param Context The execution context for the state tree.
    */
    void SetDefaultValues(FStateTreeExecutionContext& Context)
    {
        DistanceToPlayer = 0.f;
        PlayerCharacterLevel = 1;
        IsPlayerAttacking = false;
        IsPlayerDead = false;
    }

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
            "Monitors the Player"
        );
        //return NSLOCTEXT("PGAS_STEPlayer", "PlayerEvaluatorDesc", "Monitors the Player");
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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Output)
    int32 PlayerCharacterLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Output)
    bool IsPlayerAttacking = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Output)
    bool IsPlayerDead = false;
};
