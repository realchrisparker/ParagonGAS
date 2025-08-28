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
#include <Characters/Base/PGAS_CharacterBase.h>


// Begin notification logic
void UPGAS_GameplayTagNotify::NotifyBegin(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, float TotalDuration)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    CachedCharacter = Cast<APGAS_CharacterBase>(Owner);
    if (CachedCharacter)
    {
        CachedASC = CachedCharacter->GetAbilitySystemComponent();
        if (CachedASC)
        {
            // Add monitoring tag for combat windows
            if (MonitorGameplayTag.IsValid())
            {
                CachedASC->AddLooseGameplayTag(MonitorGameplayTag);

                if (bDebug)
                {
                    UE_LOG(LogTemp, Warning, TEXT("NotifyBegin: Added MonitorGameplayTag %s"), *MonitorGameplayTag.ToString());
                }
            }

            // Still apply any extra Add/Remove tags if designer wants them
            if (!AddGameplayTags.IsEmpty())
            {
                CachedASC->AddLooseGameplayTags(AddGameplayTags);
            }
            if (!RemoveGameplayTags.IsEmpty())
            {
                CachedASC->RemoveLooseGameplayTags(RemoveGameplayTags);
            }

            // Fire gameplay event if BeginNotifyTag is provided
            if (BeginNotifyTag.IsValid())
            {
                FGameplayEventData EventData;
                EventData.Instigator = Owner;
                EventData.Target = Owner;
                EventData.OptionalObject = MeshComp;
                EventData.OptionalObject2 = this;
                EventData.EventMagnitude = Magnitude;
                EventData.EventTag = BeginNotifyTag;

                CachedASC->HandleGameplayEvent(EventData.EventTag, &EventData);
            }
        }
    }    

    OnGameplayTagNotifyBeginDelegate.Broadcast(this);
}

// End notification logic
void UPGAS_GameplayTagNotify::NotifyEnd(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    if (CachedASC)
    {
        // Remove monitoring tag for combat windows
        if (MonitorGameplayTag.IsValid())
        {
            CachedASC->RemoveLooseGameplayTag(MonitorGameplayTag);

            if (bDebug)
            {
                UE_LOG(LogTemp, Warning, TEXT("NotifyEnd: Removed MonitorGameplayTag %s"), *MonitorGameplayTag.ToString());
            }
        }

        // Revert tags if specified
        if (!AddGameplayTags.IsEmpty())
        {
            CachedASC->RemoveLooseGameplayTags(AddGameplayTags);
        }
        if (!RemoveGameplayTags.IsEmpty())
        {
            CachedASC->AddLooseGameplayTags(RemoveGameplayTags);
        }

        // Fire gameplay event if EndNotifyTag is provided
        if (EndNotifyTag.IsValid())
        {
            FGameplayEventData EventData;
            EventData.Instigator = Owner;
            EventData.Target = Owner;
            EventData.OptionalObject = MeshComp;
            EventData.OptionalObject2 = this;
            EventData.EventMagnitude = Magnitude;
            EventData.EventTag = EndNotifyTag;

            CachedASC->HandleGameplayEvent(EventData.EventTag, &EventData);
        }
    }

    OnGameplayTagNotifyEndDelegate.Broadcast(this);
}
