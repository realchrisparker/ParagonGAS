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
    * Date: 6-28-2025
    * =============================================================================
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "GenericTeamAgentInterface.h"
#include <Characters/Base/PGAS_CharacterBase.h>
#include <Controllers/Player/PGAS_PlayerController.h>
#include <Game/PGAS_PlayerState.h>
#include <Game/PGAS_HUD.h>
#include <GAS/AttributeSets/PlayerCharacterAttributeSet.h>
#include <GAS/PGAS_AbilitySystemComponent.h>
#include <Components/PGAS_CombatCoreComponent.h>
#include <Components/PGAS_CombatHitboxComponent.h>
#include <Components/PGAS_CombatBlockComponent.h>
#include <Components/PGAS_CombatLockOnComponent.h>
#include <Components/PGAS_CombatDodgeComponent.h>
#include <Components/PGAS_CombatParryComponent.h>
#include <UserWidgets/PGAS_InGame_HUD.h>
#include "PGAS_PlayerCharacter.generated.h"

class UNiagaraSystem;
class USoundBase;

UCLASS()
class PARAGONGAS_API APGAS_PlayerCharacter : public APGAS_CharacterBase, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    /*
    * Functions
    */

    // Constructor
    APGAS_PlayerCharacter();

    /**
     * Returns the team ID for this character.
     * This is used for AI team-based logic.
     */
    virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

    /**
     * Returns the Player Attribute Set for this character.
     * This function retrieves the Player Attribute Set associated with the character's Ability System Component.
     */
    UPlayerCharacterAttributeSet* GetAttributeSet() const
    {
        return const_cast<UPlayerCharacterAttributeSet*>(GetAbilitySystemComponent()->GetSet<UPlayerCharacterAttributeSet>());
    }

    // Gets the health attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|GAS", meta = (DisplayName = "Get Health Attribute"))
    virtual float GetHealth() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetHealth();
        return 0.0f;
    }

    // Gets the max health attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|GAS", meta = (DisplayName = "Get Max Health Attribute"))
    virtual float GetMaxHealth() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetMaxHealth();
        return 0.0f;
    }

    // Gets the stamina attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|GAS", meta = (DisplayName = "Get Stamina Attribute"))
    virtual float GetStamina() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetStamina();
        return 0.0f;
    }

    // Gets the max stamina attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|GAS", meta = (DisplayName = "Get Max Stamina Attribute"))
    virtual float GetMaxStamina() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetMaxStamina();
        return 0.0f;
    }

    // Gets the adrenaline attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|GAS", meta = (DisplayName = "Get Adrenaline Attribute"))
    virtual float GetAdrenaline() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetAdrenaline();
        return 0.0f;
    }

    // Gets the max adrenaline attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|GAS", meta = (DisplayName = "Get Max Adrenaline Attribute"))
    virtual float GetMaxAdrenaline() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetMaxAdrenaline();
        return 0.0f;
    }

    // Gets the experience points attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|GAS", meta = (DisplayName = "Get Experience Points Attribute"))
    virtual float GetExperiencePoints() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetExperiencePoints();
        return 0.0f;
    }

    // Gets the max experience points attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|GAS", meta = (DisplayName = "Get Max Experience Points Attribute"))
    virtual float GetMaxExperiencePoints() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetMaxExperiencePoints();
        return 0.0f;
    }

    /*
     * Called when health is changed.
     * This function is called whenever the character's health changes.
     * @param DeltaValue The change in health value (positive or negative).
     * @param Instigator The actor that caused the health change (e.g., damage dealer).
     * @note This function is intended to be overridden in derived classes to handle health changes.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Player|Health", meta = (DisplayName = "On Health Changed"))
    void OnHealthChanged(float DeltaValue, AActor* Causer);

    /*
     * Called when the character dies.
     * This function is called when the character's health reaches zero.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Player|Health", meta = (DisplayName = "On Death"))
    void OnDeath();

    /*
     * Called when stamina is changed.
     * This function is called whenever the character's stamina changes.
     * @param DeltaValue The change in stamina value (positive or negative).
     * @param Instigator The actor that caused the stamina change (e.g., damage dealer).
     * @note This function is intended to be overridden in derived classes to handle stamina changes.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Player|Stamina", meta = (DisplayName = "On Stamina Changed"))
    void OnStaminaChanged(float DeltaValue, AActor* Causer);

    /*
     * Called when adrenaline is changed.
     * This function is called whenever the character's adrenaline changes.
     * @param DeltaValue The change in adrenaline value (positive or negative).
     * @param Instigator The actor that caused the adrenaline change (e.g., damage dealer).
     * @note This function is intended to be overridden in derived classes to handle adrenaline changes.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Player|Adrenaline", meta = (DisplayName = "On Adrenaline Changed"))
    void OnAdrenalineChanged(float DeltaValue, AActor* Causer);

    /*
     * Called when experience points are changed.
     * This function is called whenever the character's experience points changes.
     * @param DeltaValue The change in experience points value (positive or negative).
     * @note This function is intended to be overridden in derived classes to handle experience points changes.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Player|Experience", meta = (DisplayName = "On Experience Points Changed"))
    void OnExperiencePointsChanged(float DeltaValue);

    /*
     * Called when the character levels up.
     * This function is called when the character's level increases.
     * It can be used to trigger level-up effects, animations, or UI updates.
     * @note This function is intended to be overridden in derived classes to handle level-up events.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Player|Experience", meta = (DisplayName = "On Character Level Up"))
    void OnCharacterLevelUp();

    /*
     * Handles the character's health change
     * This function is called whenever the character's health changes.
     * @param DeltaValue The change in health value (positive or negative).
     * @param Causer The actor that caused the health change (e.g., damage dealer
     */
    virtual void HandleHealthChange(float DeltaValue, AActor* Causer);

    /*
     * Handles the character's stamina change
     * This function is called whenever the character's stamina changes.
     * @param DeltaValue The change in stamina value (positive or negative).
     * @param Causer The actor that caused the stamina change (e.g., damage dealer).
     */
    virtual void HandleStaminaChange(float DeltaValue, AActor* Causer);

    /*
     * Handles the character's adrenaline change
     * This function is called whenever the character's adrenaline changes.
     * @param DeltaValue The change in adrenaline value (positive or negative).
     * @param Causer The actor that caused the adrenaline change (e.g., damage dealer).
     */
    virtual void HandleAdrenalineChange(float DeltaValue, AActor* Causer);

    /*
     * Handles the character's experience points change
     * This function is called whenever the character's experience points change.
     * @param DeltaValue The change in experience points value (positive or negative).
     */
    virtual void HandleExperiencePointsChange(float DeltaValue);

    /*
     * Handles the character's level-up logic
     */
    virtual void HandleCharacterLevelUp();

    // Sets up default abilities for the enemy character.
    // This function can be used to set up default abilities for the enemy character.
    virtual void SetupDefaultAbilities() override;

    // Gets the current idle time of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Camera")
    float GetIdleTime() const { return IdleTime; }
    
    /**
	 * Checks if the character is dead.
	 * This function is used to determine if the character is dead.
	 * @return bool True if the character is dead, false otherwise.
	 * For event driven systems, consider using the OnDeath event. This checker is good for components like LockOnComponent.
	 */
    UFUNCTION(BlueprintCallable, Category = "Player|Health")
    bool IsDead() const
    {
        // Check health.
        if (GetHealth() <= 0)
        {
            return true;
        }

        // Check alive state.
        if (!HasGameplayTag(FGameplayTag::RequestGameplayTag("Character.State.Alive")))
        {
            return true;
        }

        return false;
    }

    // Returns the Hit Reaction Component for this character
    // This component handles hit reactions for the enemy character.
    UPGAS_CombatCoreComponent* GetCombatCoreComponent() const
    {
        return CombatCoreComponent;
    }

    // Returns the Hitbox Component for this character
    // This component handles hit detection for the character.
    UPGAS_CombatHitboxComponent* GetHitboxComponent() const
    {
        return HitboxComponent;
    }

    // Returns the LockOn Component for this character
    // This component handles lock-on targeting for the character.
    UPGAS_CombatLockOnComponent* GetLockOnComponent() const
    {
        return LockOnComponent;
    }

    // Returns the Block Component for this character
    // This component handles blocking mechanics for the character.
    UPGAS_CombatBlockComponent* GetBlockComponent() const
    {
        return BlockComponent;
    }

    // Returns the Dodge Component for this character
    // This component handles dodging (including rolling) mechanics for the character.
    UPGAS_CombatDodgeComponent* GetDodgeComponent() const
    {
        return DodgeComponent;
    }

    // Returns the Parry Component for this character
    // This component handles parrying mechanics for the character.
    UPGAS_CombatParryComponent* GetParryComponent() const
    {
        return ParryComponent;
    }

    /*
     * Properties
     */

    /** Animation Montage for idle state break */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Animations|Idle", meta = (DisplayName = "Idle Break Montage",
        ToolTip = "The animation montage to play when the character breaks out of an idle state. This allows for the character to play a specific animation montage when breaking out of an idle state. This will occur if the character has been idle for a certain 30.0f of time."))
    TObjectPtr<UAnimMontage> IdleBreakMontage;

protected:

    /*
     * Functions
     */
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called when the game stops or when the actor is destroyed
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called when the player has been possessed by a controller
    virtual void PossessedBy(AController* NewController) override;

    // Called when player state has been replicated
    virtual void OnRep_PlayerState() override;

    /**
     * Called when the Health attribute is changed.
     * @param Data The data associated with the attribute change.
     */
    virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

    /**
     * Called when the Stamina attribute is changed.
     * @param Data The data associated with the attribute change.
     */
    virtual void OnStaminaAttributeChanged(const FOnAttributeChangeData& Data);

    /**
     * Called when the Adrenaline attribute is changed.
     * @param Data The data associated with the attribute change.
     */
    virtual void OnAdrenalineAttributeChanged(const FOnAttributeChangeData& Data);

    /**
     * If you prefer to bind this in the Character (rather than a PlayerController),
     * override SetupPlayerInputComponent and bind your actions here.
    */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /** Called by the IA_Move input action to handle movement. */
    UFUNCTION()
    void MoveAction(const FInputActionInstance& Value);

    /** Called by the IA_Move input action to handle movement when started. */
    UFUNCTION()
    void MoveStartedAction(const FInputActionInstance& Value);

    /** Called by the IA_Move input action to handle movement when stopped. */
    UFUNCTION()
    void MoveStopAction(const FInputActionInstance& Value);

    /** Called by the IA_Look input action to handle looking around. */
    UFUNCTION()
    void LookAction(const FInputActionValue& Value);

    /** Called by the IA_Jump input action to handle jumping. */
    UFUNCTION()
    void JumpAction(const FInputActionValue& Value);

    /** Called by the IA_Jump input action to handle jumping when released. */
    void JumpReleaseAction(const FInputActionValue& Value);

    /** Called by the IA_Sprint input action to handle sprinting. */
    UFUNCTION()
    void SprintStartAction(const FInputActionValue& Value);

    /** Called by the IA_Sprint input action to handle sprinting when released. */
    void SprintReleaseAction(const FInputActionValue& Value);

    /** Called by the IA_LightAttack input action to handle light attacking. */
    UFUNCTION()
    void LightAttackAction(const FInputActionValue& Value);

    /** Called by the IA_HeavyAttack input action to handle heavy attacking. */
    UFUNCTION()
    void HeavyAttackAction(const FInputActionValue& Value);

    /** Called by the IA_Block input action to handle blocking when started. */
    UFUNCTION()
    void BlockStartedAction(const FInputActionInstance& Value);

    /** Called by the IA_Block input action to handle blocking when released. */
    void BlockReleaseAction(const FInputActionValue& Value);

    /** Called by the IA_LockOn input action to handle locking on to a target. */
    UFUNCTION()
    void LockOnAction(const FInputActionValue& Value);

    /** Called by the IA_Dodge input action to handle dodging. */
    UFUNCTION()
    void DodgeAction(const FInputActionInstance& Value);

    /** Called by the IA_Dodge input action to handle dodging. */
    UFUNCTION()
    void RollAction(const FInputActionInstance& Value);

    /** Called by the IA_Parry input action to handle parrying. */
    UFUNCTION()
    void ParryAction(const FInputActionInstance& Value);

    /*
     * Properties
     */

    // Reference to the owning PlayerState, exposed to Blueprints
    UPROPERTY(BlueprintReadOnly, Category = "Player|State")
    TObjectPtr<APGAS_PlayerState> CachedPlayerState;

    // Reference to the current PlayerController, exposed to Blueprints
    UPROPERTY(BlueprintReadOnly, Category = "Player|Controller")
    TObjectPtr<APGAS_PlayerController> CachedPlayerController;

    // Reference to the current PlayerController, exposed to Blueprints
    UPROPERTY(BlueprintReadOnly, Category = "Player|HUD")
    TObjectPtr<APGAS_HUD> CachedPlayerHUD;

    /** Input Mapping Context specific to this player */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta= (DisplayName = "Player Input Mapping Context",
        ToolTip = "The Input Mapping Context (IMC) asset that defines the input mappings for this player character. This IMC is added to the Enhanced Input Subsystem when the character is possessed by a controller."))
    TObjectPtr<class UInputMappingContext> PlayerInputMappingContext;

    /** Priority of this IMC when added to the subsystem */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Input Mapping Context Priority",
        ToolTip = "The priority of this Input Mapping Context (IMC) when added to the Enhanced Input Subsystem."))
    int32 IMCPriority = 0;

    /**
     * The Input Action asset for moving the character (e.g. "IA_Move").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Move"))
    TObjectPtr<UInputAction> IA_Move;

    /**
     * The Input Action asset for looking around (e.g. "IA_Look").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Look"))
    TObjectPtr<UInputAction> IA_Look;

    /**
     * The Input Action asset for jumping (e.g. "IA_Jump").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Jump"))
    TObjectPtr<UInputAction> IA_Jump;

    /**
     * The Input Action asset for moving the character in sprinting/running (e.g. "IA_Sprint").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Sprint"))
    TObjectPtr<UInputAction> IA_Sprint;

    /**
     * The Input Action asset for light attacking (e.g. "IA_LightAttack").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Light Attack"))
    TObjectPtr<UInputAction> IA_LightAttack;

    /**
     * The Input Action asset for heavy attacking (e.g. "IA_HeavyAttack").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Heavy Attack"))
    TObjectPtr<UInputAction> IA_HeavyAttack;

    /** Lock-On input action */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Lock On"))
    TObjectPtr<class UInputAction> IA_LockOn;

    /**
     * The Input Action asset for blocking (e.g. "IA_Block").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Block"))
    TObjectPtr<UInputAction> IA_Block;

    /** Dodge input action */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Dodge"))
    TObjectPtr<class UInputAction> IA_Dodge;

    /** Roll input action */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Roll"))
    TObjectPtr<class UInputAction> IA_Roll;

    /** Parry input action */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input", meta = (DisplayName = "Parry"))
    TObjectPtr<class UInputAction> IA_Parry;

    /** The team ID for this character. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",
        meta = (DisplayName = "Team ID", Tooltip = "The team ID for this character. Used for AI team-based logic.")
    )
    FGenericTeamId TeamId = FGenericTeamId(1); // 1 = Player, 2 = Enemy

private:
    
    /*
     * Properties
     */

    // Camera boom positioning the camera behind the character
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    // Follow camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    // Combat core component for handling combat-related functionality
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS Combat System", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPGAS_CombatCoreComponent> CombatCoreComponent = nullptr;

    // Combat hitbox component for handling hit detection
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS Combat System", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPGAS_CombatHitboxComponent> HitboxComponent = nullptr;

    // Combat lock-on component for handling lock-on targeting
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS Combat System", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPGAS_CombatLockOnComponent> LockOnComponent = nullptr;

    // Combat block component for handling blocking mechanics
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS Combat System", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPGAS_CombatBlockComponent> BlockComponent = nullptr;

    // Combat dodge component for handling dodging mechanics
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS Combat System", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPGAS_CombatDodgeComponent> DodgeComponent = nullptr;

    // Combat parry component for handling parrying mechanics
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS Combat System", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPGAS_CombatParryComponent> ParryComponent = nullptr;

    // Attribute Set for managing character attributes (health, mana, etc.)
    // This is where you define your character's attributes like health, mana, etc.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPlayerCharacterAttributeSet> AttributeSet;

    /**
     * Set of actors that have already been hit by the weapon trace
     * This is used to prevent hitting the same actor multiple times in a single trace.
     * It is a transient property, meaning it will not be saved or replicated.
     */
    UPROPERTY(Transient)
    TSet<TWeakObjectPtr<AActor>> AlreadyHitActors;

    // Timer handle for the weapon trace timer.
    FTimerHandle WeaponTraceTimerHandle;

    float IdleTime = 0.0f; // The time the character has been idle.
    bool bIdleAnimationPlayed = false; // Whether the idle animation has been played or not.

    //----------------
    // Lock On
    //----------------

    /** Threshold for detecting a right stick flick on X axis */
    UPROPERTY(EditDefaultsOnly, Category = "Input|LockOn")
    float StickFlickThreshold = 0.7f;

    /** Minimum delay between flick switches (prevents rapid toggling) */
    UPROPERTY(EditDefaultsOnly, Category = "Input|LockOn")
    float FlickCooldown = 0.35f;

    /** Tracks last flick time */
    float LastFlickTime = -1.f;

    /** Tracks whether stick is currently considered at rest (prevents retriggering while held) */
    bool bStickAtRest = true;

    /*
     * Functions
     */

    UFUNCTION(BlueprintCallable, Category = "Player|Animations|Idle")
    void PlayIdleBreakMontage()
    {
        // Play the idle animation montage here. Use Montage_Play, GAS, or any system you prefer.
        if (IdleBreakMontage && GetMesh())
        {
            GetMesh()->GetAnimInstance()->Montage_Play(IdleBreakMontage);
        }
    }

    // --------------------
    // Combat Window Handlers
    // --------------------

    /**
     * Called when a combat window is started
     * This function is responsible for starting hit detection in the hitbox component.
     */
    UFUNCTION()
    void HandleCombatWindowStartHanded(FPGAS_AttackType AttackData);

    /**
     * Called when a combat window is ended
     * This function is responsible for stopping hit detection in the hitbox component.
     */
    UFUNCTION()
    void HandleCombatWindowEndHanded(FPGAS_AttackType AttackData);

    /**
     * Called when the hitbox component detects a hit
     * This function processes the hit and applies damage or effects as necessary.
     * @param HitActor The actor that was hit.
     * @param Hit The hit result containing details about the hit.
     * @param AttackType The type of attack that caused the hit.
     */
    UFUNCTION()
    void HandleHitboxHit(AActor* HitActor, const FHitResult& Hit, FPGAS_AttackType AttackType);

	/*
	 * Helper function to get the hand name from the weapon hand enum
	 * @param Hand - The weapon hand enum value
	 */
	FName HandToSetName(EPGAS_WeaponHand Hand) const
	{
		switch (Hand)
		{
			case EPGAS_WeaponHand::Left:  return TEXT("LeftHand");
			case EPGAS_WeaponHand::Right: return TEXT("RightHand");
			case EPGAS_WeaponHand::Both:  return NAME_None; // NAME_None -> “all sets” in our Hitbox
			default:                      return NAME_None;
		}
    }

    /**
     * Helper function to perform an attack
     * @param AttackType - The type of attack to perform (light, heavy, etc)
     * @param ComboIndex - The index of the combo attack to perform
     */
    void PerformAttack(EPGAS_WeaponAttackType AttackType, int32 ComboIndex);

    // Plays a simple camera shake effect
    void PlaySimpleShake(float scale = 1.0f);
};