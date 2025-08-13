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
#include "Components/StateTreeAIComponent.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Enums/PGAS_StateTreeEvent.h>
#include "PGAS_STE_Perception.generated.h"

class APGAS_EnemyAIController;

/**
 * StateTree Evaluator that listens to AIController's perception events.
 */
UCLASS(Blueprintable, 
    meta = (
        DisplayName = "AI Perception Evaluator",
        category = "PGAS",
        ShortTooltip = "Evaluates the AI perception of the context actor.",
        Tooltip = "Monitors the AI perception GAS attribute of the context actor.",
        Icon = "Check_Circle")
)
class PARAGONGAS_API UPGAS_STE_Perception : public UStateTreeEvaluatorBlueprintBase
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
            "PGAS_STE_Perception",
            "PerceptionDesc",
            "Monitors the Context Actor's AI perceptions"
        );
    };

    virtual FName GetIconName() const {
        return FName("Check_Circle");
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

    /**
     * The current acquired target from perception events
     * Used in State Trees to determine the current focus of the AI and is easily bindable.
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Output, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<AActor> AcquiredTarget = nullptr;

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

    /** Sends a StateTree event with the specified tag */
    void SendEvent(const EPGAS_StateTreeEvent event)
    {
        // Get event tag based on enum value
        FGameplayTag EventTag;
        switch (event)
        {
            case EPGAS_StateTreeEvent::Idle:
                EventTag = FGameplayTag::RequestGameplayTag(FName("StateTree.Event.Idle"));
                LastEvent = EPGAS_StateTreeEvent::Idle;
                break;
            case EPGAS_StateTreeEvent::Attack:
                EventTag = FGameplayTag::RequestGameplayTag(FName("StateTree.Event.Attack"));
                LastEvent = EPGAS_StateTreeEvent::Attack;
                break;
            case EPGAS_StateTreeEvent::Patrol:
                EventTag = FGameplayTag::RequestGameplayTag(FName("StateTree.Event.Patrol"));
                LastEvent = EPGAS_StateTreeEvent::Patrol;
                break;
            default:
                return; // Ignore unknown events
        }

        // Send the event to the StateTree component if it exists
        if (AIController.IsValid() && StateTreeComp)
        {
            FStateTreeEvent Event(EventTag);
            StateTreeComp->SendStateTreeEvent(Event);
        }
    }

    /*
     * Properties
    */

    UPROPERTY()
    TObjectPtr<UStateTreeAIComponent> StateTreeComp;
    
    /** Last event sent to the StateTree */
    EPGAS_StateTreeEvent LastEvent = EPGAS_StateTreeEvent::Idle; // Track the last event sent to the StateTree
};
