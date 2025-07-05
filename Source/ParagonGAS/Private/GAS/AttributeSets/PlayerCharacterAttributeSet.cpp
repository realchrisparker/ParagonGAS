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


#include "GAS/AttributeSets/PlayerCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include <Characters/Base/PGAS_CharacterBase.h>
#include <Characters/Player/PGAS_PlayerCharacter.h>

UPlayerCharacterAttributeSet::UPlayerCharacterAttributeSet()
{
}

void UPlayerCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate the RPG attributes
    DOREPLIFETIME(UPlayerCharacterAttributeSet, Health);
    DOREPLIFETIME(UPlayerCharacterAttributeSet, MaxHealth);
    DOREPLIFETIME(UPlayerCharacterAttributeSet, Stamina);
    DOREPLIFETIME(UPlayerCharacterAttributeSet, MaxStamina);
    DOREPLIFETIME(UPlayerCharacterAttributeSet, Adrenaline);
    DOREPLIFETIME(UPlayerCharacterAttributeSet, MaxAdrenaline);
    DOREPLIFETIME(UPlayerCharacterAttributeSet, ExperiencePoints);
    DOREPLIFETIME(UPlayerCharacterAttributeSet, MaxExperiencePoints);
}

void UPlayerCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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
    APGAS_PlayerCharacter* TargetCharacter = nullptr;
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        TargetCharacter = Cast<APGAS_PlayerCharacter>(TargetActor);
    }

    // Check if the attribute being modified is one of the RPG attributes
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // Ensure health does not exceed max health
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
        if (TargetCharacter)
            TargetCharacter->HandleHealthChange(DeltaValue, Data.EffectSpec.GetContext().GetInstigator()); // Handle health change in the target character
    }

    if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
    {
        bool StaminaOverflow = GetStamina() > GetMaxStamina() ? FMath::IsNearlyEqual(GetStamina() - DeltaValue, GetMaxStamina()) : false;
        // Ensure stamina does not exceed max stamina
        SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
        if (TargetCharacter && StaminaOverflow == false)
            TargetCharacter->HandleStaminaChange(DeltaValue, Data.EffectSpec.GetContext().GetInstigator()); // Handle stamina change in the target character
    }

    if (Data.EvaluatedData.Attribute == GetAdrenalineAttribute())
    {
        // Ensure adrenaline does not exceed max adrenaline
        SetAdrenaline(FMath::Clamp(GetAdrenaline(), 0.0f, GetMaxAdrenaline()));
        if (TargetCharacter)
            TargetCharacter->HandleAdrenalineChange(DeltaValue, Data.EffectSpec.GetContext().GetInstigator()); // Handle adrenaline change in the target character
    }

    if (Data.EvaluatedData.Attribute == GetExperiencePointsAttribute())
    {
        float Difference = GetExperiencePoints() - GetMaxExperiencePoints();
        if (TargetCharacter)
        {
            // Fire the experience points change event
            TargetCharacter->HandleExperiencePointsChange(DeltaValue);

            if (Difference >= 0)
                TargetCharacter->HandleCharacterLevelUp(); // Fire the level-up event if experience points exceed max experience points
        }
    }
}

void UPlayerCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetMaxHealthAttribute())
    {
        AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
    }

    if (Attribute == GetMaxStaminaAttribute())
    {
        AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
    }

    if (Attribute == GetMaxAdrenalineAttribute())
    {
        AdjustAttributeForMaxChange(Adrenaline, MaxAdrenaline, NewValue, GetAdrenalineAttribute());
    }

    if (Attribute == GetMaxExperiencePointsAttribute())
    {
        SetExperiencePoints(0.f);
    }
}

void UPlayerCharacterAttributeSet::AdjustAttributeForMaxChange(
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

void UPlayerCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, Health, OldHealth);
}

void UPlayerCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UPlayerCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, Stamina, OldStamina);
}

void UPlayerCharacterAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, MaxStamina, OldMaxStamina);
}

void UPlayerCharacterAttributeSet::OnRep_Adrenaline(const FGameplayAttributeData& OldAdrenaline)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, Adrenaline, OldAdrenaline);
}

void UPlayerCharacterAttributeSet::OnRep_MaxAdrenaline(const FGameplayAttributeData& OldMaxAdrenaline)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, MaxAdrenaline, OldMaxAdrenaline);
}

void UPlayerCharacterAttributeSet::OnRep_ExperiencePoints(const FGameplayAttributeData& OldExperiencePoints)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, ExperiencePoints, OldExperiencePoints);
}

void UPlayerCharacterAttributeSet::OnRep_MaxExperiencePoints(const FGameplayAttributeData& OldMaxExperiencePoints)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, MaxExperiencePoints, OldMaxExperiencePoints);
}
