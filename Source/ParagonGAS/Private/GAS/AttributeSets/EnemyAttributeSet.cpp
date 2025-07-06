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
 * Date: 7-5-2025
 * =============================================================================
 * EnemyAttributeSet.h
 * This file defines the attribute set for enemy characters in the game.
 * It includes attributes like Health and MaxHealth, and provides functionality for managing these attributes.
*/


#include "GAS/AttributeSets/EnemyAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include <Characters/Base/PGAS_CharacterBase.h>
#include <Characters/Enemy/PGAS_EnemyCharacter.h>

UEnemyAttributeSet::UEnemyAttributeSet()
{
}

void UEnemyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate the RPG attributes
    DOREPLIFETIME(UEnemyAttributeSet, Health);
    DOREPLIFETIME(UEnemyAttributeSet, MaxHealth);
}

void UEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    // Set the delta value based on the operation type
    // If the operation is additive, we use the magnitude as the delta value.
    float DeltaValue = 0;
    if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
    {
        DeltaValue = Data.EvaluatedData.Magnitude;
    }

    AActor* TargetActor = nullptr;
    APGAS_EnemyCharacter* TargetCharacter = nullptr;
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        TargetCharacter = Cast<APGAS_EnemyCharacter>(TargetActor);
    }

    // Check if the attribute being modified is one of the RPG attributes
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // Ensure health does not exceed max health
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
        if (TargetCharacter)
            TargetCharacter->HandleHealthChange(DeltaValue, Data.EffectSpec.GetContext().GetInstigator()); // Handle health change in the target character
    }
}

void UEnemyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetMaxHealthAttribute())
    {
        AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
    }
}

void UEnemyAttributeSet::AdjustAttributeForMaxChange(
    FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
    // Get the owning AbilitySystemComponent
    UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();

    // Get the current value of the max attribute
    const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

    // If the new max value is nearly equal to the current max value, no adjustment is needed
    if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent)
    {
        // Adjust the affected attribute based on the new max value
        AbilitySystemComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Override, NewMaxValue);
    }
}

void UEnemyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, Health, OldHealth);
}

void UEnemyAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, MaxHealth, OldMaxHealth);
}
