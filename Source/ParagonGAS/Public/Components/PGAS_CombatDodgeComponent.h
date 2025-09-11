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
 * Date: 8-31-2025
 * =============================================================================
 * PGAS_DodgeComponent.h
 * This file defines the Dodge Component for handling dodge functionality.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Structs/PGAS_DodgeType.h>
#include "PGAS_CombatDodgeComponent.generated.h"

/*
 * Delegates
*/

/** Delegate called when a dodge is started */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDodgeStarted);

/**
 * Dodge Component
 *
 * Handles dodge & rolling input, state, and related combat functionality.
 * Initially a blank component, will be expanded with stamina cost,
 * animations, gameplay effects, etc.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (PGAS),
    meta = (BlueprintSpawnableComponent,
        DisplayName = "PGAS Dodge Component", 
        Description = "Handles dodging & rolling mechanics for the character.", 
        Tooltip = "Manages dodge & rolling input and state.")
)
class PARAGONGAS_API UPGAS_CombatDodgeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Constructor */
    UPGAS_CombatDodgeComponent();

    /*
     * Properties
     */

    /**
     * Array of possible dodge types this character can perform.
     * Each entry represents a specific dodge action.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge", meta = (DisplayName = "Dodge", Tooltip = "Array of possible dodge types this character can perform."))
    TArray<FPGAS_DodgeType> Dodges;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge", meta = (DisplayName = "Shadowing", Tooltip = "Whether performing a dodge creates a semi-transparent copy of the character during the dodge."))
    bool bShadowing = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge", meta = (DisplayName = "Slow Motion", Tooltip = "Whether performing a dodge triggers slow motion effects."))
    bool bSlowMotion = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge", meta = (DisplayName = "Debug", Tooltip = "Whether to enable debug logging for dodges."))
    bool bDebug = false;

    /*
     * Events
     */

     /** Event fired when a dodge is performed (no parameters). */
    UPROPERTY(BlueprintAssignable, Category = "Dodge")
    FOnDodgeStarted OnDodgeStarted;

    /*
     * Functions
     */

    /** Get the first dodge that matches a given category and direction. */
    UFUNCTION(BlueprintCallable, Category = "Dodge")
    FPGAS_DodgeType GetDodgeByCategoryAndDirection(EPGAS_DodgeCategory Category, EPGAS_DodgeDirection Direction) const;

    /** Get all dodges that match a given category (Dodge vs Roll). */
    UFUNCTION(BlueprintCallable, Category = "Dodge")
    TArray<FPGAS_DodgeType> GetAllDodgesByCategory(EPGAS_DodgeCategory Category) const;

    /**
	 * Perform a dodge using the specified DodgeType.
	 * Wraps the DodgeType into an object and sends it to the Ability System.
	 */
    UFUNCTION(BlueprintCallable, Category="Dodge")
    void PerformDodge(const FPGAS_DodgeType& Dodge);

protected:
    /** Called when the game starts */
    virtual void BeginPlay() override;

private:    
};
