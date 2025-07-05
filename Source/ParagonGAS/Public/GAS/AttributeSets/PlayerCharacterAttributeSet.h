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
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \

/**
 * Player Character Attribute Set
*/
UCLASS(meta=(DisplayName="Player Character Attribute Set"))
class PARAGONGAS_API UPlayerCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	/*
	 * Functions
	*/
	
	/** Default constructor */
	UPlayerCharacterAttributeSet();

	/*
	 * Override the GetLifetimeReplicatedProps function to replicate attributes.
	 * This is necessary for the Gameplay Ability System to replicate attributes to clients.
	 * The attributes will be replicated automatically by the GAS framework.
	*/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called when a gameplay effect is executed on this attribute set */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** Called before an attribute is changed */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/*
	 * Properties
	*/
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPlayerCharacterAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPlayerCharacterAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UPlayerCharacterAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UPlayerCharacterAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Adrenaline)
	FGameplayAttributeData Adrenaline;
	ATTRIBUTE_ACCESSORS(UPlayerCharacterAttributeSet, Adrenaline);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxAdrenaline)
	FGameplayAttributeData MaxAdrenaline;
	ATTRIBUTE_ACCESSORS(UPlayerCharacterAttributeSet, MaxAdrenaline);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_ExperiencePoints)
	FGameplayAttributeData ExperiencePoints;
	ATTRIBUTE_ACCESSORS(UPlayerCharacterAttributeSet, ExperiencePoints);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxExperiencePoints)
	FGameplayAttributeData MaxExperiencePoints;
	ATTRIBUTE_ACCESSORS(UPlayerCharacterAttributeSet, MaxExperiencePoints);

protected:
	/*
	 * Functions
	*/
	
	UFUNCTION()
	void AdjustAttributeForMaxChange(
		FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute,
		float NewMaxValue,
		const FGameplayAttribute& AffectedAttributeProperty);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
	virtual void OnRep_Adrenaline(const FGameplayAttributeData& OldAdrenaline);

	UFUNCTION()
	virtual void OnRep_MaxAdrenaline(const FGameplayAttributeData& OldMaxAdrenaline);

	UFUNCTION()
	virtual void OnRep_ExperiencePoints(const FGameplayAttributeData& OldExperiencePoints);

	UFUNCTION()
	virtual void OnRep_MaxExperiencePoints(const FGameplayAttributeData& OldMaxExperiencePoints);
};
