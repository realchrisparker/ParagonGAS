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
 * PGAS_BlockComponent.h
 * This component handles blocking/guarding mechanics for characters.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include <Enums/PGAS_WeaponHand.h>
#include "PGAS_CombatBlockComponent.generated.h"

/*
* Forward Declarations
*/

class APGAS_PlayerCharacter;


/*
* Delegates
*/

/** Delegate for when block state changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlockStateChangedSignature, bool, bNewIsBlocking);


/**
 * A component that manages blocking/guarding for a character.
 * This will interact with GAS to reduce incoming damage or negate attacks if blocking is active.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (PGAS),
    meta = (BlueprintSpawnableComponent,
        DisplayName = "PGAS Combat Block Component",
        Description = "Handles blocking mechanics for characters.",
        Tooltip = "Manages blocking state and interactions.")
)
class PARAGONGAS_API UPGAS_CombatBlockComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Constructor
    UPGAS_CombatBlockComponent();

    /*
     * Functions
     */

    /** Whether the character is currently blocking */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Block")
    bool bIsBlocking = false;

    /** Start blocking (called when block input pressed) */
    UFUNCTION(BlueprintCallable, Category = "Block")
    void StartBlock();

    /** Stop blocking (called when block input released) */
    UFUNCTION(BlueprintCallable, Category = "Block")
    void StopBlock();

    /*
     * Properties
     */

    /** Event broadcast whenever block state changes */
    UPROPERTY(BlueprintAssignable, Category = "Block")
    FOnBlockStateChangedSignature OnBlockStateChanged;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
    bool StopMovementOnBlock = false;

    /** Which hand is considered the blocking hand (Left, Right, or Both) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
    EPGAS_WeaponHand DominantBlockHand = EPGAS_WeaponHand::Left;

protected:

    /*
     * Functions
     */
    
    // Called when the game starts
    virtual void BeginPlay() override;

private:
	/** Cached owning character for convenience */
	UPROPERTY()
    TObjectPtr<APGAS_PlayerCharacter> OwnerCharacter;

	/** Cached AbilitySystemComponent for GAS integration */
	UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> ASC;

    FActiveGameplayEffectHandle BlockDefenseBuffHandle;
};
