/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * This file defines a base evaluator that binds to EnemyAIController's perception events.
 * Author: Christopher D. Parker
 * Date: 7-28-2025
 * =============================================================================
 * This file defines the base StateTree Evaluator that listens to AIController's perception events.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "Perception/AIPerceptionTypes.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include "PGAS_STE_PerceptionBase.generated.h"

class APGAS_EnemyAIController;

/**
 * Base StateTree Evaluator that listens to AIController's perception events.
 */
UCLASS(Blueprintable, meta= (DisplayName = "Perception Evaluator Base"))
class PARAGONGAS_API UPGAS_STE_PerceptionBase : public UStateTreeEvaluatorBlueprintBase
{
    GENERATED_BODY()

public:
    /*
     * Functions
    */

    /**
     * Initializes the evaluator and binds to the AIController's perception events.
     * This is called when the StateTree starts.
     * @param Context The execution context for the StateTree.
     */
    virtual void TreeStart(FStateTreeExecutionContext& Context) override;

    /**
     * Cleans up the evaluator and unbinds from the AIController's perception events.
     * This is called when the StateTree stops.
     * @param Context The execution context for the StateTree.
     */
    virtual void TreeStop(FStateTreeExecutionContext& Context) override;

    /*
     * Properties
    */
    
    /** The owning Enemy Character (set at TreeStart) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<APGAS_EnemyCharacter> Character;

    /** Pointer to the bound controller */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta= (AllowPrivateAccess = "true"))
    TWeakObjectPtr<APGAS_EnemyAIController> AIController;

    /** The current acquired target from perception events */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Output, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<AActor> AcquiredTarget = nullptr;

    /** The current acquired target location from perception events */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Output, meta = (AllowPrivateAccess = "true"))
    FVector AcquiredTargetLocation = FVector::ZeroVector;

protected:
    /*
     * Functions
    */
    
    /** Called when sight stimulus is detected */
    UFUNCTION(BlueprintImplementableEvent, Category = "Perception")
    void OnSightStimulus(AActor* Actor, const FAIStimulus& Stimulus);

    /** Called when sight stimulus is forgotten */
    UFUNCTION(BlueprintImplementableEvent, Category = "Perception")
    void OnSightStimulusForgotten(AActor* Actor);

    /** Called when hearing stimulus is detected */
    UFUNCTION(BlueprintImplementableEvent, Category = "Perception")
    void OnHearingStimulus(AActor* Actor, const FAIStimulus& Stimulus);

    /** Called when hearing stimulus is forgotten */
    UFUNCTION(BlueprintImplementableEvent, Category = "Perception")
    void OnHearingStimulusForgotten(AActor* Actor);

    /** Called when damage stimulus is detected */
    UFUNCTION(BlueprintImplementableEvent, Category = "Perception")
    void OnDamageStimulus(AActor* Actor, const FAIStimulus& Stimulus);

private:
    /*
     * Functions
    */    

    /** Native C++ handlers for delegate binding */
    UFUNCTION()
    void HandleSightStimulus(AActor* Actor, const FAIStimulus& Stimulus);

    UFUNCTION()
    void HandleSightStimulusForgotten(AActor* Actor);

    UFUNCTION()
    void HandleHearingStimulus(AActor* Actor, const FAIStimulus& Stimulus);

    UFUNCTION()
    void HandleHearingStimulusForgotten(AActor* Actor);

    UFUNCTION()
    void HandleDamageStimulus(AActor* Actor, const FAIStimulus& Stimulus);
};
