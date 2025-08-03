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
 * Date: 7-29-2025
 * =============================================================================
 * StateTree task to get a random reachable location within a radius around an actor.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "NavigationSystem.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_GetRandomLocation.generated.h"

UCLASS(Blueprintable, BlueprintType, Category = "PGAS", meta = (DisplayName = "Get Random Location"))
class PARAGONGAS_API UPGAS_GetRandomLocation : public UStateTreeTaskBlueprintBase
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

    // Radius in Unreal units for the search
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    float SearchRadius = 1000.f;

    // The result: a random, reachable location
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Output, meta = (Output))
    FVector RandomLocation = FVector::ZeroVector;

    /*
     * Functions
    */   

#if WITH_EDITOR
    virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif

protected:
    /*
     * Functions
    */
    
    /**
      * This function will find a random reachable location around the actor within the specified radius.
      * @param Context The execution context for the state tree.
      * @param Transition The transition result containing information about the state change.
     */
    EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
};
