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
 * Date: 9-6-2025
 * =============================================================================
 * PGAS_CombatParryComponent.h
 * This component handles combat parry functionality such as timing windows,
 * event dispatching, and integration with GAS.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "PGAS_CombatParryComponent.generated.h"


/*
 * Delegates
 */

// Delegate for parry success
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnParrySuccessSignature);

// Delegate for parry failure
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnParryFailedSignature);

/**
 * Combat Parry Component
 * This component handles the parry mechanics for the character.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (PGAS),
    meta = (BlueprintSpawnableComponent,
        DisplayName = "PGAS Combat Parry Component",
        Description = "Handles parry mechanics for characters.",
        Tooltip = "Manages parry state and interactions.")
)
class PARAGONGAS_API UPGAS_CombatParryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPGAS_CombatParryComponent();

    /*
     * Functions
     */

    /** Starts a parry attempt (called when player presses parry input) */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Parry")
    void StartParry();

    /** Called when parry is successful */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Parry")
    void OnParrySuccess(AActor* Attacker);

    /** Called when parry fails (missed window or mistimed) */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Combat|Parry")
    void OnParryFailed();

    /** Returns true if the component is currently in the active parry window */
    UFUNCTION(BlueprintPure, Category = "PGAS|Combat|Parry")
    bool IsInActiveWindow() const { return bIsInActiveWindow; }

    /*
     * Properties
     */
     
    /** Duration of startup before parry becomes active */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Parry", meta = (AllowPrivateAccess = "true"))
    float StartupDuration = 0.15f;

    /** Duration of active parry window */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Parry", meta = (AllowPrivateAccess = "true"))
    float ActiveWindowDuration = 0.25f;

    /** Duration of recovery after parry window */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Parry", meta = (AllowPrivateAccess = "true"))
    float RecoveryDuration = 0.4f;

    /** GameplayTag that identifies the Parry ability to activate */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PGAS|Combat|Parry", meta = (AllowPrivateAccess = "true"))
    FGameplayTag ParryAbilityTag;

    /** Broadcast when parry succeeds */
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Parry")
    FOnParrySuccessSignature OnParrySuccessDelegate;

    /** Broadcast when parry fails */
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Combat|Parry")
    FOnParryFailedSignature OnParryFailedDelegate;

protected:
    virtual void BeginPlay() override;

private:
    /*
     * Functions
     */

    /** Internal helpers */
    void EnterActiveWindow();
    void EnterRecoveryPhase();

    /*
     * Properties
     */

    /** Internal state */
    FTimerHandle ParryStartupHandle;
    FTimerHandle ParryActiveHandle;
    FTimerHandle ParryRecoveryHandle;

    bool bIsParrying = false;
    bool bIsInActiveWindow = false;
};
