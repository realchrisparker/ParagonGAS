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
#include "PGAS_WeaponDataAsset.generated.h"

/**
 * Hitbox profile for weapon attacks, defining the start and end sockets, radius, and max hit count.
 */
<<<<<<< HEAD
USTRUCT(BlueprintType, meta=(DisplayName="Hitbox Profile"))
=======
USTRUCT(BlueprintType)
>>>>>>> 0d025d5a21a8ae2ea10a969535b24b3e069911f3
struct PARAGONGAS_API FPGAS_HitboxProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hitbox")
    FName StartSocket;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hitbox")
    FName EndSocket;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hitbox")
    float Radius = 15.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hitbox")
    int32 MaxHitCount = 1;
<<<<<<< HEAD

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hitbox")
    bool bDebugDraw = false;
=======
>>>>>>> 0d025d5a21a8ae2ea10a969535b24b3e069911f3
};

/**
 * Hand profile for weapon attacks, defining the attack montages, GAS tags, and hitbox profile.
 */
<<<<<<< HEAD
USTRUCT(BlueprintType, meta=(DisplayName="Weapon Hand Profile"))
=======
USTRUCT(BlueprintType)
>>>>>>> 0d025d5a21a8ae2ea10a969535b24b3e069911f3
struct PARAGONGAS_API FPGAS_WeaponHandProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
    TObjectPtr<UAnimMontage> LightAttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
    TObjectPtr<UAnimMontage> HeavyAttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
    TObjectPtr<UAnimMontage> BlockMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
    FGameplayTag LightAttackAbilityTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
    FGameplayTag HeavyAttackAbilityTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
    FGameplayTag BlockAbilityTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
    TArray<TSubclassOf<UGameplayEffect>> OnHitEffects;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
    TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hitbox")
    FPGAS_HitboxProfile HitboxProfile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Costs")
    float LightAttackStaminaCost = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Costs")
    float HeavyAttackStaminaCost = 25.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    float LightAttackDamage = 15.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    float HeavyAttackDamage = 35.f;
};

/**
 * Struct returned by query helper
 */
<<<<<<< HEAD
USTRUCT(BlueprintType, meta=(DisplayName="Weapon Attack Data"))
=======
USTRUCT(BlueprintType)
>>>>>>> 0d025d5a21a8ae2ea10a969535b24b3e069911f3
struct PARAGONGAS_API FPGAS_AttackData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Attack")
    float Damage = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Attack")
    float StaminaCost = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Attack")
    TObjectPtr<UAnimMontage> AttackMontage;

    UPROPERTY(BlueprintReadOnly, Category = "Attack")
    TArray<TSubclassOf<UGameplayEffect>> OnHitEffects;

    UPROPERTY(BlueprintReadOnly, Category = "Attack")
    FGameplayTag AbilityTag;
};

/**
 * Weapon data asset class, holding all weapon-related data.
 */
UCLASS(BlueprintType, meta=(DisplayName="Weapon Data Asset"))
class PARAGONGAS_API UPGAS_WeaponDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    /*
    * Properties
    */

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    FText WeaponName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TObjectPtr<UTexture2D> WeaponIcon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
    TObjectPtr<USkeletalMesh> WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
    TObjectPtr<UAnimMontage> EquipMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
    TObjectPtr<UAnimMontage> UnequipMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Profiles")
    FPGAS_WeaponHandProfile LeftHandProfile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Profiles")
    FPGAS_WeaponHandProfile RightHandProfile;

    /*
    * Functions
    */

    /** Existing helpers */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    float GetAttackDamage(bool bIsLeftHand, bool bIsHeavyAttack) const;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    float GetAttackStaminaCost(bool bIsLeftHand, bool bIsHeavyAttack) const;

    /** New combined query helper */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    FPGAS_AttackData GetAttackData(bool bIsLeftHand, bool bIsHeavyAttack) const;

    /** Override for primary asset ID */
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(TEXT("Weapon"), GetFName());
    }
};
