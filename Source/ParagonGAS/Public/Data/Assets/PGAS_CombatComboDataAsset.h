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
 * Date: 9-1-2025
 * =============================================================================
 * PGAS_ComboDataAsset.h
 * This file defines the data structures and asset used to define combat attack combos.
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "AbilitySystemComponent.h"
#include "PGAS_CombatComboDataAsset.generated.h"

#if WITH_EDITORONLY_DATA
class UComboGraph;               // forward declare editor graph
#endif


/*
 * Enums
 */

/** Result of executing a combo step. */
UENUM(BlueprintType)
enum class ECombatComboResult : uint8
{
    None        UMETA(DisplayName = "None"),
    Success     UMETA(DisplayName = "Success"),
    Failure     UMETA(DisplayName = "Failure")
};

/*
 * Structs
 */

 /**
  * Represents a single step in a combo chain.
  */
USTRUCT(BlueprintType, Category = "Combat Combo", meta = (DisplayName = "Combat Combo Step"))
struct PARAGONGAS_API FPGAS_CombatCombo
{
    GENERATED_BODY()

    FPGAS_CombatCombo()
        : MontageToPlay(nullptr)
        // , InputWindowTime(0.4f)
        // , PreviousStepIndex(INDEX_NONE)
        // , SuccessStepIndex(INDEX_NONE)
        // , FailStepIndex(INDEX_NONE)
    {
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    FName Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    FGameplayTag TriggerTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    FGameplayTag AbilityTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    TObjectPtr<UAnimMontage> MontageToPlay;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    // float InputWindowTime;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    // TArray<int32> NextStepIndices;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    // int32 PreviousStepIndex;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    // int32 SuccessStepIndex;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    // int32 FailStepIndex;

    // bool CanTransitionTo(int32 StepIndex) const { return NextStepIndices.Contains(StepIndex); }
    bool IsAbilityBased() const { return AbilityTag.IsValid(); }
    bool IsMontageBased() const { return !AbilityTag.IsValid() && MontageToPlay != nullptr; }
};


/**
 * A combo sequence starting with a root (like Light Chain, Heavy Chain, Air Combo)
 */
USTRUCT(BlueprintType)
struct FPGAS_CombatComboSequence
{
    GENERATED_BODY()

    FPGAS_CombatComboSequence()
        : SequenceName(NAME_None)
        , RootStepIndex(0)
    {
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    FName SequenceName;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    // TArray<FPGAS_CombatCombo> Combos;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 RootStepIndex;
};


/**
 * Data asset that defines a combo chain.
 */
UCLASS(BlueprintType, Blueprintable, Category = "Combat Combo", meta = (DisplayName = "Combat Combo Graph"))
class PARAGONGAS_API UPGAS_CombatComboGraph : public UDataAsset
{
    GENERATED_BODY()

public:
    /*
    * Properties
    */

    /** Name/identifier for this combo combat data asset (helpful for debugging/UI). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (DisplayName = "Graph Name"))
    FName GraphName;

    /** All sequences defined in this graph */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (DisplayName = "Sequences"))
    TArray<FPGAS_CombatComboSequence> Sequences;

#if WITH_EDITORONLY_DATA
    /** Editor-only graph that stores node layout and wiring */
    UComboGraph* EditorGraph = nullptr;  // raw pointer, not nested type
#endif
};
