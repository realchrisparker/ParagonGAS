
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

#include "Characters/Enemy/PGAS_EnemyCharacter.h"
#include "GAS/Abilities/PGAS_ReceiveStaffMeleeDamageAbility.h"

APGAS_EnemyCharacter::APGAS_EnemyCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Set up the character's attribute set.
    // This is where you define your character's attributes like health, mana, etc.
    AttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("AttributeSet"));

    // Default character level
    SetCharacterLevel(1);

    // Add default gameplay tags.
    SetupDefaultGameplayTags();
}

// Called when the game starts or when spawned
void APGAS_EnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Apply default attribute effects.
    ApplyDefaultAttributeEffects();

    // Set up default abilities for the enemy character.
    SetupDefaultAbilities();
}

// Called every frame
void APGAS_EnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

/*
 * SetupDefaultGameplayTags function to set up default gameplay tags for this character.
 * This is typically called in the constructor or BeginPlay.
*/
void APGAS_EnemyCharacter::SetupDefaultGameplayTags()
{
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Type.Enemy")));
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Alive")));
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove")));
}

// Handles changes to the character's health attribute
void APGAS_EnemyCharacter::HandleHealthChange(float DeltaValue, AActor* Causer)
{
    OnHealthChanged(DeltaValue, Causer);
    if (GetHealth() <= 0)
    {
        // If the character's health is zero or less, trigger the death event
        OnDeath();
    }
}
