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
 * Date: 7-22-2025
 * =============================================================================
 * Implementation of custom StateTree Component for enhanced debugging and control.
 */

#include <Components/PGAS_StateTreeAIComponent.h>
#include "StateTreeSchema.h"


UPGAS_StateTreeAIComponent::UPGAS_StateTreeAIComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    SetStartLogicAutomatically(false);  // Do not start logic automatically on BeginPlay
}

void UPGAS_StateTreeAIComponent::BeginPlay()
{
    Super::BeginPlay();
    // UE_LOG(LogTemp, Warning, TEXT("PGAS_StateTreeComponent::BeginPlay"));
}

void UPGAS_StateTreeAIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPGAS_StateTreeAIComponent::InitializeComponent()
{
    Super::InitializeComponent();
    // UE_LOG(LogTemp, Warning, TEXT("PGAS_StateTreeComponent::InitializeComponent"));
}

void UPGAS_StateTreeAIComponent::StartLogic()
{
    // UE_LOG(LogTemp, Warning, TEXT("PGAS_StateTreeComponent::StartLogic"));
    Super::StartLogic();
}

void UPGAS_StateTreeAIComponent::StopLogic(const FString& Reason)
{
    // UE_LOG(LogTemp, Warning, TEXT("PGAS_StateTreeComponent::StopLogic called with reason: %s"), *Reason);
    Super::StopLogic(Reason);
}

void UPGAS_StateTreeAIComponent::ValidateStateTreeReference()
{
    // UE_LOG(LogTemp, Warning, TEXT("PGAS_StateTreeComponent::ValidateStateTreeReference"));
    Super::ValidateStateTreeReference();
}

void UPGAS_StateTreeAIComponent::SetStateTree(UStateTree* StateTree)
{
    UE_LOG(LogTemp, Warning, TEXT("PGAS_StateTreeComponent::SetStateTree called"));
    if (!StateTree)
    {
        UE_LOG(LogTemp, Error, TEXT("PGAS_StateTreeComponent::SetStateTree - StateTree is NULL!"));
        return;
    }

    Super::SetStateTree(StateTree);
}

void UPGAS_StateTreeAIComponent::StartStateTree(UStateTree* StateTree)
{
    if (StateTree)
    {
        if (IsRunning())
        {
            Cleanup();
        }

        StateTreeRef.SetStateTree(StateTree);
        StartLogic();
    }
}

void UPGAS_StateTreeAIComponent::SendEvent(const FGameplayTag& EventTag)
{
    if (IsRunning())
    {
        FStateTreeEvent Event = FStateTreeEvent(EventTag);
        SendStateTreeEvent(Event);
        UE_LOG(LogTemp, Warning, TEXT("UPGAS_StateTreeAIComponent::SendEvent - Event '%s' sent."), *EventTag.GetTagName().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UPGAS_StateTreeAIComponent::SendEvent - StateTree execution context is not valid. Event '%s' not sent."), *EventTag.GetTagName().ToString());
    }
}

// /**
//  * Show a notification in the editor.
//  * @param Message The message to display in the notification.
//  * @param State The completion state of the notification (Success, Fail, etc.).
//  */
// void UPGAS_StateTreeAIComponent::ShowEditorNotification(const FText& Message, SNotificationItem::ECompletionState State)
// {
//     FNotificationInfo Info(Message);
//     Info.FadeInDuration = 0.2f;
//     Info.FadeOutDuration = 0.5f;
//     Info.ExpireDuration = 3.0f;
//     Info.bUseLargeFont = false;
//     Info.bFireAndForget = true; // auto-dismiss
//     Info.bUseThrobber = false;
//     Info.bUseSuccessFailIcons = true;

//     TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info);
//     if (Notification.IsValid())
//     {
//         Notification->SetCompletionState(State);
//     }
// }
// #if WITH_EDITOR
// // Show editor notification
// ShowEditorNotification(FText::FromString(FString::Printf(TEXT("StateTree parameter 'Motivation' updated to %s."),
//     *NewMotivation.ToString())), SNotificationItem::ECompletionState::CS_Success);
//#endif