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
    * Date: 6-28-2025
    * =============================================================================
*/

#include "Animations/PGAS_GameplayTagNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "Logging/LogMacros.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPGAS_GameplayTagNotify::UPGAS_GameplayTagNotify()
{
}

void UPGAS_GameplayTagNotify::NotifyBegin(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, float TotalDuration)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    // Check required tag
    if (RequiredTag.IsValid())
    {
        const IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Owner);
        if (TagInterface && !TagInterface->HasMatchingGameplayTag(RequiredTag))
        {
            return; // Do not trigger if RequiredTag is not present
        }
    }

    // Add/Remove tags if AbilitySystemComponent (ASC) exists
    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
    {
        ASC->AddLooseGameplayTags(AddGameplayTags);
        ASC->RemoveLooseGameplayTags(RemoveGameplayTags);

        // Construct a payload
        // This is useful for notifying the ASC about the start of a combo or attack sequence
        FGameplayEventData EventData;
        EventData.Instigator = Owner;
        EventData.Target = Owner; // Assuming the target is the same as the instigator
        EventData.OptionalObject = MeshComp; // Optional object can be the mesh component
        EventData.OptionalObject2 = this; // Optional object can be the notify itself
        EventData.EventMagnitude = Magnitude; // Optional magnitude
        EventData.EventTag = BeginNotifyTag;

        // Broadcast the event to the ASC
        // This allows the ASC to handle any gameplay logic associated with the start of the notify
        ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
    }

    OnGameplayTagNotifyBeginDelegate.Broadcast(this);
}

void UPGAS_GameplayTagNotify::NotifyEnd(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    // Revert tags if AbilitySystemComponent (ASC) exists
    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
    {
        ASC->RemoveLooseGameplayTags(AddGameplayTags);
        ASC->AddLooseGameplayTags(RemoveGameplayTags);

        // Construct a payload
        // This is useful for notifying the ASC about the end of a combo or attack sequence
        FGameplayEventData EventData;
        EventData.Instigator = Owner;
        EventData.Target = Owner; // Assuming the target is the same as the instigator
        EventData.OptionalObject = MeshComp; // Optional object can be the mesh component
        EventData.OptionalObject2 = this; // Optional object can be the notify itself
        EventData.EventMagnitude = Magnitude; // Optional magnitude
        EventData.EventTag = EndNotifyTag;

        // Broadcast the event to the ASC
        // This allows the ASC to handle any gameplay logic associated with the end of the notify
        ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
    }

    OnGameplayTagNotifyEndDelegate.Broadcast(this);
}

FString UPGAS_GameplayTagNotify::GetNotifyName_Implementation() const
{
    return Id.IsEmpty() ? GetNotifyName() : Id;
}
