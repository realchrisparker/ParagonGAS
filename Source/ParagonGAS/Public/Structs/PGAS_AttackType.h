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
 * Date: 8-17-2025
 * =============================================================================
 * Attack type struct
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include <Enums/PGAS_WeaponHand.h>
#include <Data/Assets/PGAS_WeaponDataAsset.h>
#include <Structs/PGAS_AttackHitbox.h>
#include <GAS/Effects/PGAS_GE_MeleeDamageEffect.h>
#include "PGAS_AttackType.generated.h"


/**
 * Represents an attack type in the PGAS Combat System
 */
USTRUCT(BlueprintType, Blueprintable, meta = (DisplayName = "PGAS Attack Type", Description = "Represents an attack type in the PGAS Combat System"))
struct PARAGONGAS_API FPGAS_AttackType
{
    GENERATED_BODY()

public:
    /*
    * Properties
    */

    // The gameplay tag that identifies the ability or attack
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (DisplayName = "Ability Tag", Description = "The gameplay tag that identifies the ability or attack"))
    FGameplayTag AbilityTag;

    // Which hand performs this attack (left/right/both)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (DisplayName = "Attack Hand", Description = "Which hand performs this attack (left/right/both)"))
    EPGAS_WeaponHand Hand = EPGAS_WeaponHand::Right;

    // The weapon data asset associated with this attack
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowedClasses = "PGAS_WeaponDataAsset", DisplayName = "Weapon Data", Description = "The weapon data asset associated with this attack"))
    TObjectPtr<UPGAS_WeaponDataAsset> WeaponData = nullptr;

    // The type of attack (light, medium, heavy)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (DisplayName = "Attack Type", Description = "The type of attack (light, medium, heavy, etc.)"))
    EPGAS_WeaponAttackType AttackType = EPGAS_WeaponAttackType::Light;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (DisplayName = "Hitbox", Description = "The hitbox information for this attack"))
    FPGAS_AttackHitbox Hitbox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (DisplayName = "Stamina Cost", Description = "The stamina cost of this attack"))
    float BaseStaminaCost = 0.f;

    /** Gameplay effect used for damage application (Default to PGAS_GE_MeleeDamageEffect but can be changed)*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Damage")
    TSubclassOf<UGameplayEffect> DamageEffect = UPGAS_GE_MeleeDamageEffect::StaticClass();
};