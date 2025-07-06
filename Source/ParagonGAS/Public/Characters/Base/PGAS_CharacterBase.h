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
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PGAS_CharacterBase.generated.h"

UCLASS()
class PARAGONGAS_API APGAS_CharacterBase : public ACharacter, public IAbilitySystemInterface//, public IICharacterCombat
{
	GENERATED_BODY()

public:
	/*
	* Functions
	*/
	
	// Sets default values for this character's properties
	APGAS_CharacterBase();

	/** Adds a gameplay tag to this character */
	UFUNCTION(BlueprintCallable, Category = "Character|Gameplay Tags")
	void AddGameplayTag(FGameplayTag Tag);

	/** Removes a gameplay tag from this character */
	UFUNCTION(BlueprintCallable, Category = "Character|Gameplay Tags")
	void RemoveGameplayTag(FGameplayTag Tag);

	/** Returns all the gameplay tags for this character */
	UFUNCTION(BlueprintCallable, Category = "Character|Gameplay Tags")
	FGameplayTagContainer GetGameplayTags() const;

	/** Checks if character has a specific gameplay tag */
	UFUNCTION(BlueprintCallable, Category = "Character|Gameplay Tags")
	bool HasGameplayTag(FGameplayTag Tag) const;

	/** Checks if character has all tags from a container */
	UFUNCTION(BlueprintCallable, Category = "Character|Gameplay Tags")
	bool HasAllGameplayTags(const FGameplayTagContainer& TagContainer) const;

	/** Checks if character has any tag from a container */
	UFUNCTION(BlueprintCallable, Category = "Character|Gameplay Tags")
	bool HasAnyGameplayTag(const FGameplayTagContainer& TagContainer) const;

	/*
	* Activates abilities with the specified gameplay tags.
	* @param AbilityTags The gameplay tags of the abilities to activate.
	* @param AllowRemoteActivation Whether to allow remote activation of abilities (default: true).
	* @return bool True if any abilities were activated, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "Character|Abilities", meta = (DisplayName = "Activate Abilities With Tags"))
	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation = true);
	
	// Applies default attribute effects to the character
	// This function is typically called in the BeginPlay or constructor to set up the character's
	virtual void ApplyDefaultAttributeEffects();

	// Removes default attribute effects from the character
	// This function can be used to remove default attribute effects that were applied at spawn.
	virtual void RemoveDefaultAttributeEffects();

	// Sets up default abilities for the enemy character.
	// This function can be used to set up default abilities for the enemy character.
	virtual void SetupDefaultAbilities();

	/** Returns the character level */
	UFUNCTION(BlueprintCallable, Category = "Character")
	int32 GetCharacterLevel() const { return CharacterLevel; }

	/** Sets the character level */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetCharacterLevel(int32 NewLevel) { CharacterLevel = NewLevel; }

	/*
	* Properties
	*/

	// Returns the Ability System Component for this character
	// This is required for the Ability System Interface (IAbilitySystemInterface)
	UAbilitySystemComponent* GetAbilitySystemComponent() const
	{
		return AbilitySystemComponent;
	}

	/* Default Abilities */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Abilities",
		meta = (DisplayName = "Default Abilities", Description = "Default abilities granted to the character at spawn."))
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	/* Default Attribute Effects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Attributes",
		meta = (DisplayName = "Default Attribute Effects", Description = "Default attribute effects applied to the character at spawn."))
	TArray<TSubclassOf<class UGameplayEffect>> DefaultAttributeEffects;

	/**
	 * Checks if the character is currently attacking.
	 * This function is used to determine if the character is in an attacking state.
	 * @return bool True if the character is attacking, false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Character|Combat", meta = (DisplayName = "Is Currently Attacking", HideSelfPin = "true", ReturnDisplayName = "Is Attacking",
		ToolTip = "Checks if the character is currently attacking."))
	bool IsAttacking() const
	{
		return bIsAttacking;
	}

	/**
	 * Sets the attacking state of the character.
	 * This function is used to set whether the character is currently attacking or not.
	 * @param bAttacking True if the character is attacking, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Combat", meta = (DisplayName = "Set Is Attacking", ToolTip = "Sets the attacking state of the character."))
	void SetIsAttacking(bool bAttacking)
	{
		bIsAttacking = bAttacking;
		UE_LOG(LogTemp, Warning, TEXT("Set Is Attacking: %s"), bIsAttacking ? TEXT("True") : TEXT("False"));
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/*
	* Properties
	*/

	// Ability System Component for managing abilities and effects
	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComponent;

	// Character's level, used for scaling abilities and attributes
	// This can be used to determine the character's power level, abilities, etc.
	UPROPERTY(VisibleAnywhere, Category = "Player|Attributes")
	int32 CharacterLevel;

	// Flag to check if the character is currently attacking
	bool bIsAttacking = false;
};
