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
 * Date: 7-22-2025
 * =============================================================================
 * Custom StateTree Component for enhanced debugging and control.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeAIComponent.h"
#include "StateTreeInstanceData.h"
#include "GameplayTagContainer.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "StateTreeEvents.h"
#include "StateTreeExecutionContext.h"
#include "PGAS_StateTreeAIComponent.generated.h"

UCLASS(ClassGroup = (PGAS), meta = (BlueprintSpawnableComponent, DisplayName = "PGAS State Tree AI Component"))
class PARAGONGAS_API UPGAS_StateTreeAIComponent : public UStateTreeAIComponent
{
    GENERATED_BODY()

public:
    /*
     * Functions
    */

    // Constructor
    UPGAS_StateTreeAIComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Called when the game starts or when spawned */
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Called when the component is initialized */
    virtual void InitializeComponent() override;

    /** Validates the State Tree reference */
    virtual void ValidateStateTreeReference() override;

    /**
     * Sets the State Tree reference for this component and starts the logic.
     * @param StateTreeReference The State Tree reference to set.
    */
    UFUNCTION(BlueprintCallable, Category = "StateTree", meta = (AutoCreateRefTerm = "StateTreeReference", DisplayName = "Start State Tree"))
    void StartStateTree(UStateTree* StateTree);

    /**
     * Stops the currently running State Tree logic.
     */
    UFUNCTION(BlueprintCallable, Category = "StateTree", meta = (DisplayName = "Stop State Tree"))
    void StopStateTree();

    /**
     * Sends an event to the State Tree.
     * @param EventTag The gameplay tag representing the event to send.
    */
    UFUNCTION(BlueprintCallable, Category = "StateTree", meta = (AutoCreateRefTerm = "EventTag", DisplayName = "Send Event"))
    void SendEvent(const FGameplayTag& EventTag);

private:

    /*
     * Properties
    */

    /** The last event tag sent to the State Tree */
    FGameplayTag LastEventTag = FGameplayTag::EmptyTag;
};
 