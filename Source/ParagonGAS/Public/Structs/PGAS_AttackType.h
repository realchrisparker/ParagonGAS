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

    // The combo step index for this attack. 
    // Example: 0 = first attack in chain, 1 = second, etc.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Combo",
        meta = (DisplayName = "Combo Index",
            Description = "The combo step index for this attack. Example: 0 = first attack in chain, 1 = second, etc.",
            ToolTip = "Defines the order of this attack in a combo chain."))
    int32 ComboIndex = 0;

    // Whether this attack causes hit stop (brief pause on hit)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|HitStop",
        meta = (DisplayName = "Hit Stop",
            Description = "Whether this attack causes hit stop (brief pause on hit)",
            ToolTip = "If true, the game will briefly pause when this attack hits an opponent to enhance impact."))
    bool HitStop = false;

    // Duration of hit stop in seconds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|HitStop",
        meta = (EditCondition = "HitStop",
            DisplayName = "Hit Stop Duration",
            Description = "Duration of hit stop in seconds",
            ToolTip = "Specifies how long the hit stop effect lasts when this attack hits an opponent."))
    float HitStopDuration = 0.05f;

    // Whether this attack causes camera shake on hit
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|CameraShake",
        meta = (DisplayName = "Causes Camera Shake",
            Description = "Whether this attack causes camera shake on hit",
            ToolTip = "If true, the camera will shake when this attack hits an opponent to enhance impact."))
    bool CausesCameraShake = false;
};