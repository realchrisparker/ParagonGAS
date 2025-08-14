// PGAS_DebugFormattedTextTask.h
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
 * Date: 7-31-2025
 * =============================================================================
 * A StateTree task that takes a heterogenous array of parameters (float, int, string, name, text)
 * and displays a formatted debug string at an actor’s location (or the world origin).
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "PGAS_DebugFormattedText.generated.h"


UENUM(BlueprintType, Category = "PGAS|Debug")
enum class EPGAS_DebugParamType : uint8
{
    Float   UMETA(DisplayName = "Float"),
    Int     UMETA(DisplayName = "Int"),
    String  UMETA(DisplayName = "String"),
    Name    UMETA(DisplayName = "Name"),
    Text    UMETA(DisplayName = "Text"),
    Bool    UMETA(DisplayName = "Bool"),
};

USTRUCT(BlueprintType, Category = "PGAS|Debug")
struct FPGAS_DebugFormatParam
{
    GENERATED_BODY()

    /** The placeholder key in your FormatText, e.g. "Health" for "{Health}" */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    FName Key;

    /** Which value slot to use */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    EPGAS_DebugParamType Type = EPGAS_DebugParamType::Float;

    /** Only valid if Type==Float */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    float FloatValue = 0.f;

    /** Only valid if Type==Int */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    int32 IntValue = 0;

    /** Only valid if Type==String */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    FString StringValue;

    /** Only valid if Type==Name */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    FName NameValue;

    /** Only valid if Type==Text */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    FText TextValue;

    /** Only valid if Type==Bool */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    bool BoolValue;
};

/**
 * Displays a formatted debug string using a printf-style format.
 * Example FormatString: "Health: %.2f"
 * Parameter will be substituted for the “%f”.
 */
UCLASS(meta = (DisplayName = "Debug Formatted Text", Category = "PGAS", Description = "Displays a formatted debug text at an actor's location (or origin)."))
class PARAGONGAS_API UPGAS_DebugFormattedText : public UStateTreeTaskBlueprintBase
{
    GENERATED_BODY()

public:
    /**
     * Printf-style format.
     * E.g. TEXT("Health: %.2f / %.2f")
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
    FText FormatText = NSLOCTEXT("PGAS", "DebugFmt", "Type Something {Value}");

    /** Single float parameter to be inserted into FormatString */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
    TArray<FPGAS_DebugFormatParam> Params;

    /** Optional actor to attach the debug text to.
        If null, world origin (0,0,0) is used. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<AActor> Actor = nullptr;

    /** If true, also print the message via AddOnScreenDebugMessage in black */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task", meta = (DisplayName = "Print To Screen"))
    bool PrintToScreen = false;

#if WITH_EDITOR

    /**
     * Gets the description of the condition for editor display.
     * @param ID The unique identifier for the node.
     * @param InstanceDataView The data view for the instance.
     * @param BindingLookup The binding lookup for the state tree.
     * @param Formatting The formatting options for the description.
     * @return The formatted description text.
    */
    virtual FText GetDescription(
        const FGuid& ID,
        FStateTreeDataView InstanceDataView,
        const IStateTreeBindingLookup& BindingLookup,
        EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const {
        return NSLOCTEXT(
            "PGAS_STEHealth",
            "HealthEvaluatorDesc",
            "Display Formatted Debug Text at Actor Location"
        );
    };

    virtual FName GetIconName() const {
        return FName("Info_Circle");
    };

#endif

protected:
    /**
      * This function will find a random reachable location around the actor within the specified radius.
      * @param Context The execution context for the state tree.
      * @param Transition The transition result containing information about the state change.
     */
    EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
};
