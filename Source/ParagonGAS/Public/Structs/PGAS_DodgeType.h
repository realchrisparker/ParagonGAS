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
 * Date: 8-31-2025
 * =============================================================================
 * PGAS_DodgeType.h
 * This struct defines the data for a Dodge Type, similar to PGAS_AttackType.
 * Currently empty, but intended for expansion (ability tags, montage, stamina cost, etc.).
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "PGAS_DodgeType.generated.h"


 /**
  * Enums for Dodge Direction and Category
  */
UENUM(BlueprintType)
enum class EPGAS_DodgeDirection : uint8
{
    Forward,
    Backward,
    Left,
    Right
};

/**
 * Dodge Categories
 */
UENUM(BlueprintType)
enum class EPGAS_DodgeCategory : uint8
{
    Dodge,
    Roll
};

/**
  * Data structure that defines a Dodge Type.
  * This will be used by the Dodge Component to hold all dodge options
  * a character can perform. It will later include properties such as
  * ability tags, direction, stamina cost, montage references, etc.
  */
USTRUCT(BlueprintType, Blueprintable, 
    meta = (DisplayName = "PGAS Dodge Type", 
        Description = "Represents a dodge type in the PGAS Combat System")
)
struct PARAGONGAS_API FPGAS_DodgeType
{
    GENERATED_BODY()

public:
    /** What category this action belongs to (dodge vs roll). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
    EPGAS_DodgeCategory DodgeCategory = EPGAS_DodgeCategory::Dodge;

    /** Which direction this dodge moves the character. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
    EPGAS_DodgeDirection Direction = EPGAS_DodgeDirection::Forward;

    /** Animation montage to play for this dodge. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
    TObjectPtr<UAnimMontage> Montage;

    /** GAS tag used to activate an ability (optional). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
    FGameplayTag AbilityTag = FGameplayTag::RequestGameplayTag("Character.Ability.Dodge");

    /** Stamina (or other attribute) cost of this dodge. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
    float StaminaCost = 5.f;
};

/**
 * PGAS Dodge Type Wrapper
 */
UCLASS(BlueprintType, Blueprintable,
    meta = (DisplayName = "PGAS Dodge Type Wrapper", 
        Description = "Represents a wrapper for dodge type in the PGAS Combat System")
)
class PARAGONGAS_API UPGAS_DodgeTypeObject : public UObject
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
    FPGAS_DodgeType DodgeType;
};