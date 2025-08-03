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
 * PGAS_PerformAttack
 * A StateTree task that selects and triggers a randomized attack ability via GAS.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "GameplayTagContainer.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_PerformAttack.generated.h"


/**
 * Modes for selecting which attack ability to fire
*/
UENUM(BlueprintType)
enum class EPGAS_AttackSelectMode : uint8
{
    Sequential UMETA(DisplayName = "Sequential"),
    Random     UMETA(DisplayName = "Random")
};

UCLASS(Blueprintable, BlueprintType, Category = "PGAS", meta = (DisplayName = "Perform Attack"))
class PARAGONGAS_API UPGAS_PerformAttackTask : public UStateTreeTaskBlueprintBase
{
    GENERATED_BODY()

public:
    /*
     * Properties
    */

    /** The enemy character this task is associated with. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyCharacter> Actor;

    /** The AI controller managing the enemy character. */
    UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
    TObjectPtr<APGAS_EnemyAIController> AIController;

    /** Gameplay tags for the different attack abilities to choose from */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (DisplayName = "Attack Ability Tags"))
    TArray<FGameplayTag> AttackAbilityTags;

    /** Selection mode for choosing abilities */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (DisplayName = "Selection Mode"))
    EPGAS_AttackSelectMode SelectionMode = EPGAS_AttackSelectMode::Random;

protected:
    /*
     * Functions
    */
    
    // Called when entering the state
    EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
};