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
#include <Characters/Base/PGAS_CharacterBase.h>
#include <Controllers/Player/PGAS_PlayerController.h>
#include <Game/PGAS_PlayerState.h>
#include <Game/PGAS_HUD.h>
#include <GAS/AttributeSets/PlayerCharacterAttributeSet.h>
#include <GAS/PGAS_AbilitySystemComponent.h>
#include "PGAS_PlayerCharacter.generated.h"

UCLASS()
class PARAGONGAS_API APGAS_PlayerCharacter : public APGAS_CharacterBase
{
    GENERATED_BODY()

public:
    /*
    * Functions
    */

    // Constructor
    APGAS_PlayerCharacter();

    /**
     * Activates the character's melee ability
     * This function checks if the character can activate the melee ability and performs the activation.
     * @param AllowRemoteActivation Whether to allow remote activation of the ability (default: true
    */
    UFUNCTION(BlueprintCallable, Category = "Character|Abilities|Melee",
        meta = (DisplayName = "Activate Melee Ability", ToolTip = "Activates the character's melee ability. This function checks if the character can activate the melee ability and performs the activation."))
    bool ActivateMeleeAbility(bool AllowRemoteActivation = true);

    /**
     * Returns the Player Attribute Set for this character.
     * This function retrieves the Player Attribute Set associated with the character's Ability System Component.
     */
    UPlayerCharacterAttributeSet* GetAttributeSet() const
    {
        return const_cast<UPlayerCharacterAttributeSet*>(GetAbilitySystemComponent()->GetSet<UPlayerCharacterAttributeSet>());
    }

    // Gets the health attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Attributes", meta = (DisplayName = "Get Health Attribute"))
    virtual float GetHealth() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetHealth();
        return 0.0f;
    }

    // Gets the max health attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Attributes", meta = (DisplayName = "Get Max Health Attribute"))
    virtual float GetMaxHealth() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetMaxHealth();
        return 0.0f;
    }

    // Gets the stamina attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Attributes", meta = (DisplayName = "Get Stamina Attribute"))
    virtual float GetStamina() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetStamina();
        return 0.0f;
    }

    // Gets the max stamina attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Attributes", meta = (DisplayName = "Get Max Stamina Attribute"))
    virtual float GetMaxStamina() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetMaxStamina();
        return 0.0f;
    }

    // Gets the adrenaline attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Attributes", meta = (DisplayName = "Get Adrenaline Attribute"))
    virtual float GetAdrenaline() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetAdrenaline();
        return 0.0f;
    }

    // Gets the max adrenaline attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Attributes", meta = (DisplayName = "Get Max Adrenaline Attribute"))
    virtual float GetMaxAdrenaline() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetMaxAdrenaline();
        return 0.0f;
    }

    // Gets the experience points attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Attributes", meta = (DisplayName = "Get Experience Points Attribute"))
    virtual float GetExperiencePoints() const
    {
        const UPlayerCharacterAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetExperiencePoints();
        return 0.0f;
    }

    // Gets the max experience points attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Attributes", meta = (DisplayName = "Get Max Experience Points Attribute"))
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

    UFUNCTION(BlueprintCallable, Category = "Player|Combat",
        meta = (AllowPrivateAccess = "true",
            DisplayName = "Weapon Trace",
            Keywords = "combat trace weapon hit detection",
            Tooltip = "Performs a weapon trace to detect hits."))
    void WeaponTrace();

    // Sets up default abilities for the enemy character.
    // This function can be used to set up default abilities for the enemy character.
    virtual void SetupDefaultAbilities() override;

protected:
    /*
    * Functions
    */
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called when the player has been possessed by a controller
    virtual void PossessedBy(AController* NewController) override;

    // Called when player state has been replicated
    virtual void OnRep_PlayerState() override;

    /**
     * If you prefer to bind this in the Character (rather than a PlayerController),
     * override SetupPlayerInputComponent and bind your actions here.
    */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Handles the montage state notify events.
    // This function is called when a montage state notify event occurs.
    UFUNCTION()
    void HandleMontageStateNotify(FGameplayTag NotifyTag, FGameplayEventData EventData);

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

    /** Called by the IA_PrimaryAttack input action to handle primary attacking. */
    UFUNCTION()
    void PrimaryAttackAction(const FInputActionValue& Value);

    /*
    * Properties
    */

    // Reference to the owning PlayerState, exposed to Blueprints
    UPROPERTY(BlueprintReadOnly, Category = "Player|State")
    TObjectPtr<APGAS_PlayerState> MyPlayerState;

    // Reference to the current PlayerController, exposed to Blueprints
    UPROPERTY(BlueprintReadOnly, Category = "Player|Controller")
    TObjectPtr<APGAS_PlayerController> MyPlayerController;

    // Reference to the current PlayerController, exposed to Blueprints
    UPROPERTY(BlueprintReadOnly, Category = "Player|HUD")
    TObjectPtr<APGAS_HUD> MyPlayerHUD;

    // Melee ability class to be used by the character.
    // This is typically set in the editor or loaded in code.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Abilities|Melee")
    TSubclassOf<class UGameplayAbility> MeleeAbility;

    /**
     * The Input Action asset for moving the character (e.g. "IA_Move").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
    TObjectPtr<UInputAction> IA_Move;

    /**
     * The Input Action asset for looking around (e.g. "IA_Look").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
    TObjectPtr<UInputAction> IA_Look;

    /**
     * The Input Action asset for jumping (e.g. "IA_Jump").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
    TObjectPtr<UInputAction> IA_Jump;

    /**
     * The Input Action asset for primary attacking (e.g. "IA_PrimaryAttack").
     * This is typically set in the editor or loaded in code.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
    TObjectPtr<UInputAction> IA_PrimaryAttack;

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

    // Attribute Set for managing character attributes (health, mana, etc.)
    // This is where you define your character's attributes like health, mana, etc.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPlayerCharacterAttributeSet> AttributeSet;

    UPROPERTY()
    FGameplayAbilitySpecHandle MeleeAbilitySpecHandle;

    /**
     * Set of actors that have already been hit by the weapon trace
     * This is used to prevent hitting the same actor multiple times in a single trace.
     * It is a transient property, meaning it will not be saved or replicated.
    */
    UPROPERTY(Transient)
    TSet<TWeakObjectPtr<AActor>> AlreadyHitActors;

    FName WeaponStaffStartSocketName = "staff_start"; // The name of the weapon staff start socket.
    FName WeaponStaffEndSocketName = "staff_end"; // The name of the weapon staff end socket.

    // Timer handle for the weapon trace timer.
    FTimerHandle WeaponTraceTimerHandle;

    /*
    * Functions
    */

    // Sets up the default gameplay tags for this character.
    // This is typically called in the constructor or BeginPlay.
    void SetupDefaultGameplayTags();

    FVector GetStaffStartSocketLocation() const
    {
        // Make sure we have a valid mesh and the socket exists before trying to get the location.
        if (!GetMesh())
        {
            UE_LOG(LogTemp, Warning, TEXT("GetMesh() returned nullptr in GetStaffStartSocketLocation()"));
            return FVector::ZeroVector; // Return zero vector if mesh is not valid.
        }

        // Check if the socket exists before trying to get its location.
        if (!GetMesh()->DoesSocketExist(WeaponStaffStartSocketName))
        {
            UE_LOG(LogTemp, Warning, TEXT("Socket '%s' does not exist on the mesh in GetStaffStartSocketLocation()"), *WeaponStaffStartSocketName.ToString());
            return FVector::ZeroVector; // Return zero vector if socket does not exist.
        }

        // Get the location of the staff start socket on the character's mesh. In world location.		
        return GetMesh()->GetSocketLocation(WeaponStaffStartSocketName);
    }

    FVector GetStaffEndSocketLocation() const
    {
        // Make sure we have a valid mesh and the socket exists before trying to get the location.
        if (!GetMesh())
        {
            UE_LOG(LogTemp, Warning, TEXT("GetMesh() returned nullptr in GetStaffEndSocketLocation()"));
            return FVector::ZeroVector; // Return zero vector if mesh is not valid.
        }

        // Check if the socket exists before trying to get its location.
        if (!GetMesh()->DoesSocketExist(WeaponStaffEndSocketName))
        {
            UE_LOG(LogTemp, Warning, TEXT("Socket '%s' does not exist on the mesh in GetStaffEndSocketLocation()"), *WeaponStaffEndSocketName.ToString());
            return FVector::ZeroVector; // Return zero vector if socket does not exist.
        }

        return GetMesh()->GetSocketLocation(WeaponStaffEndSocketName);
    }
};