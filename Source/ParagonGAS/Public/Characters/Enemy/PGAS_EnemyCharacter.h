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
* PGAS_EnemyCharacter.h
* This file defines the enemy character class in the game.
*/

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StateTreeReference.h"
#include <GAS/AttributeSets/EnemyAttributeSet.h>
#include <GAS/PGAS_AbilitySystemComponent.h>
#include <Characters/Base/PGAS_CharacterBase.h>
#include <Weapons/Base/PGAS_WeaponBase.h>
#include "PGAS_EnemyCharacter.generated.h"

UCLASS()
class PARAGONGAS_API APGAS_EnemyCharacter : public APGAS_CharacterBase
{
    GENERATED_BODY()

public:
    /*
    * Functions
    */

    // Constructor
    APGAS_EnemyCharacter();

    /**
     * Returns the Player Attribute Set for this character.
     * This function retrieves the Player Attribute Set associated with the character's Ability System Component.
    */
    UEnemyAttributeSet* GetAttributeSet() const
    {
        return const_cast<UEnemyAttributeSet*>(GetAbilitySystemComponent()->GetSet<UEnemyAttributeSet>());
    }

    // Gets the health attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Attributes", meta = (DisplayName = "Get Health Attribute"))
    virtual float GetHealth() const
    {
        const UEnemyAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetHealth();
        return 0.0f;
    }

    // Gets the max health attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Player|Attributes", meta = (DisplayName = "Get Max Health Attribute"))
    virtual float GetMaxHealth() const
    {
        const UEnemyAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetMaxHealth();
        return 0.0f;
    }

    /*
     * Handles the enemy's health change
     * This function is called whenever the enemy's health changes.
     * @param DeltaValue The change in health value (positive or negative).
     * @param Causer The actor that caused the health change (e.g., damage dealer).
    */
    virtual void HandleHealthChange(float DeltaValue, AActor* Causer);

    // Returns the State Tree asset for this character.
    // This is used for AI behavior logic.
    UFUNCTION(BlueprintCallable, Category = "Player|AI")
    UStateTree* GetStateTree() { return StateTree; }

    /*
    * Properties
    */

    /** The default weapon class to spawn and equip (set this to your Broadsword BP derived from APGAS_WeaponBase) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Combat", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class APGAS_WeaponBase> DefaultWeaponClass;

    /** The currently equipped weapon instance */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Combat", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class APGAS_WeaponBase> EquippedWeapon;

    /**
     * Equips the default weapon for this character. (Spawn)
     */
    UFUNCTION(BlueprintCallable, Category = "Player|Combat")
    void EquipDefaultWeapon();

    /**
     * Equips the default weapon for this character. (Spawn)
     */
    UFUNCTION(BlueprintCallable, Category = "Player|Combat")
    void ArmDefaultWeapon(EPGASWeaponInventorySlot Slot = EPGASWeaponInventorySlot::RightHand)
    {
        if (EquippedWeapon)
        {
            EquippedWeapon->InventorySlot = Slot; // Set the inventory slot for the equipped weapon, should be one of the hands.
            EquippedWeapon->EquipToMesh(this->GetMesh());
        }
    }

    /**
     * Checks if the character has a weapon equipped.
     */
    UFUNCTION(BlueprintPure, Category = "Player|Combat", meta=(ReturnDisplayName="Is Armed"))
    bool IsWeaponEquipped()
    {
        if (EquippedWeapon == nullptr)
        {
            return false;
        }

        // Determine if weapon is located in the hand of the character. If so it is equipped. If not it is unequipped.
        if (EquippedWeapon->EquipedSocketName == FName("weapon_l") || EquippedWeapon->EquipedSocketName == FName("weapon_r"))
        {
            return true;
        }

        return false;
    }

protected:
    /*
    * Functions
    */

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:

    // Attribute Set for managing enemy attributes (health, mana, etc.)
    // This is where you define your enemy's attributes like health, mana, etc.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|GAS", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UEnemyAttributeSet> AttributeSet;

    // Reference to the StateTree asset for AI logic
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|AI", meta = (AllowPrivateAccess = "true"))
    UStateTree* StateTree;

    FName HealthbarSocketName = "healthbar_Socket"; // The name of the health bar socket.

    FName WeaponStartSocketName = "weapon_start"; // The name of the weapon staff start socket.
    FName WeaponEndSocketName = "weapon_end"; // The name of the weapon staff end socket.

    /*
    * Functions
    */

    // Sets up the default gameplay tags for this character.
    // This is typically called in the constructor or BeginPlay.
    void SetupDefaultGameplayTags();

    FVector GetWeaponStartSocketLocation() const
    {
        // Make sure we have a valid mesh and the socket exists before trying to get the location.
        if (!GetMesh())
        {
            UE_LOG(LogTemp, Warning, TEXT("GetMesh() returned nullptr in GetWeaponStartSocketLocation()"));
            return FVector::ZeroVector; // Return zero vector if mesh is not valid.
        }

        // Check if the socket exists before trying to get its location.
        if (!GetMesh()->DoesSocketExist(WeaponStartSocketName))
        {
            UE_LOG(LogTemp, Warning, TEXT("Socket '%s' does not exist on the mesh in GetWeaponStartSocketLocation()"), *WeaponStartSocketName.ToString());
            return FVector::ZeroVector; // Return zero vector if socket does not exist.
        }

        // Get the location of the weapon start socket on the character's mesh. In world location.
        return GetMesh()->GetSocketLocation(WeaponStartSocketName);
    }

    FVector GetWeaponEndSocketLocation() const
    {
        // Make sure we have a valid mesh and the socket exists before trying to get the location.
        if (!GetMesh())
        {
            UE_LOG(LogTemp, Warning, TEXT("GetMesh() returned nullptr in GetWeaponEndSocketLocation()"));
            return FVector::ZeroVector; // Return zero vector if mesh is not valid.
        }

        // Check if the socket exists before trying to get its location.
        if (!GetMesh()->DoesSocketExist(WeaponEndSocketName))
        {
            UE_LOG(LogTemp, Warning, TEXT("Socket '%s' does not exist on the mesh in GetWeaponEndSocketLocation()"), *WeaponEndSocketName.ToString());
            return FVector::ZeroVector; // Return zero vector if socket does not exist.
        }

        return GetMesh()->GetSocketLocation(WeaponEndSocketName);
    }

    FVector GetHealthbarSocketLocation() const
    {
        // Make sure we have a valid mesh and the socket exists before trying to get the location.
        if (!GetMesh())
        {
            UE_LOG(LogTemp, Warning, TEXT("GetMesh() returned nullptr in GetHealthbarSocketLocation()"));
            return FVector::ZeroVector; // Return zero vector if mesh is not valid.
        }

        // Check if the socket exists before trying to get its location.
        if (!GetMesh()->DoesSocketExist(HealthbarSocketName))
        {
            UE_LOG(LogTemp, Warning, TEXT("Socket '%s' does not exist on the mesh in GetHealthbarSocketLocation()"), *HealthbarSocketName.ToString());
            return FVector::ZeroVector; // Return zero vector if socket does not exist.
        }

        // Get the location of the health bar socket on the character's mesh. In world location.
        return GetMesh()->GetSocketLocation(HealthbarSocketName);
    }
};