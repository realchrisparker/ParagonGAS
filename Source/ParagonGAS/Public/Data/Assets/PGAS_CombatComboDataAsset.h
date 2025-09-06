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
        , InputWindowTime(0.4f)
        , PreviousStepIndex(INDEX_NONE)
        , SuccessStepIndex(INDEX_NONE)
        , FailStepIndex(INDEX_NONE)
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    float InputWindowTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    TArray<int32> NextStepIndices;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 PreviousStepIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 SuccessStepIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 FailStepIndex;

    bool CanTransitionTo(int32 StepIndex) const { return NextStepIndices.Contains(StepIndex); }
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

// UCLASS(BlueprintType, Blueprintable, Category = "Combat Combo", meta = (DisplayName = "Combat Combo Graph"))
// class PARAGONGAS_API UPGAS_CombatComboGraph : public UDataAsset
// {
//     GENERATED_BODY()

// public:
    
//     // Constructor
//     UPGAS_CombatComboGraph()
//     {
//     }


//     /*
//     * Properties
//     */

//     /** Name/identifier for this combo combat data asset (helpful for debugging/UI). */
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (DisplayName = "Graph Name"))
//     FName GraphName;

//     /** All sequences defined in this graph */
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (DisplayName = "Sequences"))
//     TArray<FPGAS_CombatComboSequence> Sequences;

// #if WITH_EDITORONLY_DATA
//     /** Editor-only graph that stores node layout and wiring */
//     UComboGraph* EditorGraph = nullptr;  // raw pointer, not nested type
// #endif
    
//     /*
//     * Functions
//     */

//     // -----------------------
//     // Query Functions
//     // -----------------------

//     /** Get a sequence by index */
//     UFUNCTION(BlueprintCallable, Category = "Combo")
//     FPGAS_CombatComboSequence GetSequence(int32 Index) const
//     {
//         return Sequences.IsValidIndex(Index) ? Sequences[Index] : FPGAS_CombatComboSequence();
//     }

//     /** Get root step of a sequence */
//     // UFUNCTION(BlueprintCallable, Category = "Combo")
//     // FPGAS_CombatCombo GetRootStep(int32 SequenceIndex) const
//     // {
//     //     if (!Sequences.IsValidIndex(SequenceIndex)) return FPGAS_CombatCombo();
//     //     const FPGAS_CombatComboSequence& Seq = Sequences[SequenceIndex];
//     //     return Seq.Combos.IsValidIndex(Seq.RootStepIndex) ? Seq.Combos[Seq.RootStepIndex] : FPGAS_CombatCombo();
//     // }

//     // -----------------------
//     // Runtime Functions (Only used during runtime by Combo Component or blueprint)
//     // -----------------------

//     /** Public setter for ASC reference. */
//     UFUNCTION(BlueprintCallable, Category = "Combo", meta = (DisplayName = "Set Ability System Component"))
//     void SetAbilitySystemComponent(class UAbilitySystemComponent* InASC)
//     {
//         CachedAbilitySystemComponent = InASC;
//     }

//     /** Public setter for AnimInstance reference. */
//     UFUNCTION(BlueprintCallable, Category = "Combo", meta = (DisplayName = "Set Anim Instance"))
//     void SetAnimInstance(class UAnimInstance* InAnimInstance)
//     {
//         CachedAnimInstance = InAnimInstance;
//     }

//     // -----------------------
//     // Execution Functions
//     // -----------------------

//     /** Execute a combo step by index (chooses between GAS or Montage). */
//     UFUNCTION(BlueprintCallable, Category = "Combo", meta = (DisplayName = "Execute Combo"))
//     ECombatComboResult ExecuteCombo(int32 SequenceIndex, int32 StepIndex, bool& bOutChained, int32& OutNextIndex)
//     {
//         bOutChained = false;
//         OutNextIndex = INDEX_NONE;

//         if (!Sequences.IsValidIndex(SequenceIndex)) return ECombatComboResult::Failure;
//         FPGAS_CombatComboSequence& Seq = Sequences[SequenceIndex];
//         if (!Seq.Combos.IsValidIndex(StepIndex)) return ECombatComboResult::Failure;

//         const FPGAS_CombatCombo& Step = Seq.Combos[StepIndex];

//         // GAS-based execution
//         if (Step.IsAbilityBased() && CachedAbilitySystemComponent)
//         {
//             OnComboExecuting.Broadcast(Step);
//             if (CachedAbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(Step.AbilityTag), true))
//             {
//                 OnComboSucceeded.Broadcast(Step);
//                 OutNextIndex = Step.SuccessStepIndex;
//                 bOutChained = OutNextIndex != INDEX_NONE;
//                 return ECombatComboResult::Success;
//             }
//         }
//         // Montage-based execution
//         else if (Step.IsMontageBased() && CachedAnimInstance)
//         {
//             OnComboExecuting.Broadcast(Step);
//             if (CachedAnimInstance->Montage_Play(Step.MontageToPlay) > 0.f)
//             {
//                 OnComboSucceeded.Broadcast(Step);
//                 OutNextIndex = Step.SuccessStepIndex;
//                 bOutChained = OutNextIndex != INDEX_NONE;
//                 return ECombatComboResult::Success;
//             }
//         }

//         // Fail path
//         OnComboFailed.Broadcast(Step);
//         OutNextIndex = Step.FailStepIndex;
//         bOutChained = OutNextIndex != INDEX_NONE;
//         return ECombatComboResult::Failure;
//     }

// private:
//     /*
//     * Properties
//     */

//     /** Runtime reference to owning Ability System Component. */
//     UPROPERTY()
//     TObjectPtr<UAbilitySystemComponent> CachedAbilitySystemComponent;

//     /** Runtime reference to owning AnimInstance (from Mesh). */
//     UPROPERTY()
//     TObjectPtr<UAnimInstance> CachedAnimInstance;
// };
