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
#include <GAS/AttributeSets/EnemyAttributeSet.h>
#include <GAS/PGAS_AbilitySystemComponent.h>
#include <Characters/Base/PGAS_CharacterBase.h>
#include <GAS/PGAS_AbilitySystemComponent.h>
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
    UFUNCTION(BlueprintCallable, Category = "Enemy|Attributes", meta = (DisplayName = "Get Health Attribute"))
    virtual float GetHealth() const
    {
        const UEnemyAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetHealth();
        return 0.0f;
    }

    // Gets the max health attribute of the character.
    UFUNCTION(BlueprintCallable, Category = "Enemy|Attributes", meta = (DisplayName = "Get Max Health Attribute"))
    virtual float GetMaxHealth() const
    {
        const UEnemyAttributeSet* AttriSet = GetAttributeSet();
        if (AttriSet)
            return AttriSet->GetMaxHealth();
        return 0.0f;
    }

    /*
    * Called when health is changed.
    * This function is called whenever the enemy's health changes.
    * @param DeltaValue The change in health value (positive or negative).
    * @param Instigator The actor that caused the health change (e.g., damage dealer).
    * @note This function is intended to be overridden in derived classes to handle health changes.
    */
    UFUNCTION(BlueprintImplementableEvent, Category = "Enemy|Health", meta = (DisplayName = "On Health Changed"))
    void OnHealthChanged(float DeltaValue, AActor* Causer);

    /*
    * Called when the enemy dies.
    * This function is called when the enemy's health reaches zero.
    */
    UFUNCTION(BlueprintImplementableEvent, Category = "Enemy|Health", meta = (DisplayName = "On Death"))
    void OnDeath();

    /*
     * Handles the enemy's health change
     * This function is called whenever the enemy's health changes.
     * @param DeltaValue The change in health value (positive or negative).
     * @param Causer The actor that caused the health change (e.g., damage dealer).
    */
    virtual void HandleHealthChange(float DeltaValue, AActor* Causer);

protected:
    /*
    * Functions
    */
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    /*
    * Properties
    */

    // Attribute Set for managing enemy attributes (health, mana, etc.)
    // This is where you define your enemy's attributes like health, mana, etc.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UEnemyAttributeSet> AttributeSet;

    FName HealthbarSocketName = "healthbar_Socket"; // The name of the health bar socket.

    /*
    * Functions
    */

    // Sets up the default gameplay tags for this character.
    // This is typically called in the constructor or BeginPlay.
    void SetupDefaultGameplayTags();

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