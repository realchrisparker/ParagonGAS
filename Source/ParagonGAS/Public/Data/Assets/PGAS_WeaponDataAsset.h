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
 * Date: 8-24-2025
 * =============================================================================
 * Defines a data-driven schema for weapon configurations, enabling dual-wielding,
 * hitbox profiles, attack montages, gameplay effects, and GAS tags.
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "PGAS_WeaponDataAsset.generated.h"

/*
 * Forward Declarations
 */
class UNiagaraSystem;
class USoundBase;


/**
 * Weapon data asset class, holding all weapon-related data.
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Weapon Data Asset"))
class PARAGONGAS_API UPGAS_WeaponDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    /*
    * Properties
    */

    UPROPERTY(EditAnywhere, Category = "UI", meta = (DisplayName = "Weapon Name"))
    FText WeaponName;

    UPROPERTY(EditAnywhere, Category = "UI", meta = (DisplayName = "Weapon Icon"))
    TObjectPtr<UTexture2D> WeaponIcon;

    UPROPERTY(EditAnywhere, Category = "Visuals", meta = (DisplayName = "Weapon Mesh"))
    TObjectPtr<USkeletalMesh> WeaponMesh;

    UPROPERTY(EditAnywhere, Category = "Montages", meta = (DisplayName = "Equip Montage"))
    TObjectPtr<UAnimMontage> EquipMontage;

    UPROPERTY(EditAnywhere, Category = "Montages", meta = (DisplayName = "Unequip Montage"))
    TObjectPtr<UAnimMontage> UnequipMontage;

    UPROPERTY(EditAnywhere, Category = "Combat", meta = (DisplayName = "Base Damage", Description = "The base damage of this weapon"))
    float BaseDamage = 10.f;

    // Gameplay effects applied while weapon is equipped
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    TArray<TSubclassOf<UGameplayEffect>> WeaponEffects;   

    /** FX to play when we land a hit (particle system, Niagara, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|FX")
    UNiagaraSystem* HitImpactFX;

    /** Sound to play when hit lands */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|FX")
    USoundCue* HitImpactSound;

    /** FX to play when we swing the weapon (particle system, Niagara, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|FX")
    UNiagaraSystem* SwingFX;

    /** Sound to play when swinging the weapon */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|FX")
    USoundCue* SwingSound;

    /** The defense buff value to apply while blocking with this weapon */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Block",
        meta = (DisplayName = "Block Buff Value", ToolTip = "How much defense this weapon grants while blocking."))
    float BlockBuffValue = 0.f;

    /** Override for primary asset ID */
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(TEXT("Weapon"), GetFName());
    }
};
