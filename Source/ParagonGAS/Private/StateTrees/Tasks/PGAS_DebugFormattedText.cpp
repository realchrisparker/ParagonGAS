// PGAS_DebugFormattedTextTask.cpp
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

#include <StateTrees/Tasks/PGAS_DebugFormattedText.h>
#include "DrawDebugHelpers.h"
#include "StateTreeExecutionContext.h"

 /**
  * This function will find a random reachable location around the actor within the specified radius.
  * @param Context The execution context for the state tree.
  * @param Transition The transition result containing information about the state change.
 */
EStateTreeRunStatus UPGAS_DebugFormattedText::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    // Resolve world
    UWorld* World = Actor
        ? Actor->GetWorld()
        : Context.GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UPGAS_DebugFormattedText: No valid World!"));
        return EStateTreeRunStatus::Failed;
    }

    // Build named arguments
    FFormatNamedArguments Args;
    for (const FPGAS_DebugFormatParam& P : Params)
    {
        const FString KeyString = P.Key.ToString();

        switch (P.Type)
        {
            case EPGAS_DebugParamType::Float:
                Args.Add(KeyString, FFormatArgumentValue(P.FloatValue));
                break;

            case EPGAS_DebugParamType::Int:
                Args.Add(KeyString, FFormatArgumentValue(P.IntValue));
                break;

            case EPGAS_DebugParamType::String:
                // Convert FString → FText → FFormatArgumentValue
                Args.Add(KeyString, FFormatArgumentValue(FText::FromString(P.StringValue)));
                break;

            case EPGAS_DebugParamType::Name:
                // Convert FName → FText → FFormatArgumentValue
                Args.Add(KeyString, FFormatArgumentValue(FText::FromName(P.NameValue)));
                break;

            case EPGAS_DebugParamType::Text:
                // Already an FText
                Args.Add(KeyString, FFormatArgumentValue(P.TextValue));
                break;

            case EPGAS_DebugParamType::Bool:
                Args.Add(KeyString, FFormatArgumentValue(P.BoolValue));
        }
    }

    // Format the text
    const FText FormattedText = FText::Format(FormatText, Args);
    const FString Message = FormattedText.ToString();

    // Pick a draw location
    const FVector DrawLoc = Actor
        ? Actor->GetActorLocation()
        : FVector::ZeroVector;

    // Draw (Duration=0 → one frame)
    DrawDebugString(World, DrawLoc, Message, Actor.Get(), FColor::White, /*Duration=*/2.f, /*bDrawShadow=*/true);

    // Optionally print to screen (black, two seconds)
    if (PrintToScreen && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            /*Key=*/-1,
            /*TimeToDisplay=*/2.0f,
            /*Color=*/FColor::Black,
            /*Message=*/Message
        );
    }

    return EStateTreeRunStatus::Succeeded;
}
