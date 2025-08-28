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
    * Custom AnimNotifyState that adds/removes gameplay tags to actors during animation states. Supports conditions and Blueprint delegates.
    * This notify state is designed to work with Unreal Engine's Gameplay Ability System (GAS) and provides a flexible way to manage gameplay
    * tags during animations. It has two systems within. It can use GameplayTags for windowing and Gameplay Events for triggering specific actions.
*/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "PGAS_GameplayTagNotify.generated.h"


/*
 * Forward Declarations
 */

class UAbilitySystemComponent;
class APGAS_CharacterBase;

/*
    * Delegates
*/

// Delegate called when the notify begins
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayTagNotifyBegin, UPGAS_GameplayTagNotify*, NotifyInstance);

// Delegate called when the notify ends
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayTagNotifyEnd, UPGAS_GameplayTagNotify*, NotifyInstance);

/*
 * Gameplay Tag Notify State
 * This notify state adds/removes gameplay tags to/from the actor during animation states.
 * It supports conditions based on required tags and provides Blueprint delegates for begin/end events.
 */
UCLASS(Blueprintable,
    meta = (DisplayName = "PGAS Gameplay Tag Notify State", 
        Description = "Triggers gameplay events and manages gameplay tags during animation states.",
        Tooltip = "Triggers gameplay events and manages gameplay tags during animation states.",
        Keywords = "Gameplay, Tag, Notify, Animation"
    )
)
class PARAGONGAS_API UPGAS_GameplayTagNotify : public UAnimNotifyState
{
    GENERATED_BODY()

public:

    /*
     * Functions
    */

    // Begin notification logic
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

    // End notification logic
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    // Name displayed in editor
    virtual FString GetNotifyName_Implementation() const override { return Id.IsEmpty() ? GetNotifyName() : Id; }

    // Color displayed in editor (optional, only works if you implement editor extension)
    virtual FLinearColor GetEditorColor() override { return NotifyColor; }

    /*
     * Properties
    */

    // Notify name for identification
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify",
        meta = (DisplayName = "Notify Id", ToolTip = "Unique identifier for this notify. Used in code/editor."))
    FString Id = TEXT("Enter Notify Id Here");

    // Enable debug print
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify",
        meta = (DisplayName = "Debug Print", ToolTip = "Print debug messages when notify triggers."))
    bool bDebug = false;

    // Gameplay tags to add to the actor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|Gameplay",
        meta = (Categories = "Notify|GameplayTags", DisplayName = "Add Gameplay Tags",
            ToolTip = "Tags to add to the actor when this notify triggers. Removed when notify ends."))
    FGameplayTagContainer AddGameplayTags;

    // Gameplay tags to remove from the actor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|Gameplay",
        meta = (Categories = "Notify|GameplayTags", DisplayName = "Remove Gameplay Tags",
            ToolTip = "Tags to remove from the actor when this notify triggers. Added back when notify ends."))
    FGameplayTagContainer RemoveGameplayTags;

    // Tag that must be present for this notify to activate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|Gameplay",
        meta = (DisplayName = "Required Tag", ToolTip = "Actor must have this tag for notify to trigger."))
    FGameplayTag RequiredTag;

    // Tag that must be present for this notify to activate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|Events",
        meta = (DisplayName = "Begin Notify Tag", ToolTip = "Gameplay tag used to signal the beginning of the notify."))
    FGameplayTag BeginNotifyTag;

    // Tag that must be present for this notify to activate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|Events",
        meta = (DisplayName = "End Notify Tag", ToolTip = "Gameplay tag used to signal the end of the notify."))
    FGameplayTag EndNotifyTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|Events",
        meta = (DisplayName = "Magnitude", ToolTip = "Magnitude of the notify event."))
    float Magnitude = 1.0f;

    // Gameplay tag that this notify will add/remove for monitoring windows
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|Windowing",
        meta = (DisplayName = "Monitor Tag", ToolTip = "Tag to add on NotifyBegin and remove on NotifyEnd. Used by CombatCoreComponent to track windows."))
    FGameplayTag MonitorGameplayTag;

    //--------------
    // Events
    //--------------

    // Delegate: called on notify begin
    UPROPERTY(BlueprintAssignable, Category = "Notify")
    FOnGameplayTagNotifyBegin OnGameplayTagNotifyBeginDelegate;

    // Delegate: called on notify end
    UPROPERTY(BlueprintAssignable, Category = "Notify")
    FOnGameplayTagNotifyEnd OnGameplayTagNotifyEndDelegate;

private:

    /*
     * Properties
    */

    // ---------------------------
    // Cached owner pointers
    // ---------------------------

    TObjectPtr<APGAS_CharacterBase> CachedCharacter; // Cached reference to the owning character
    TObjectPtr<UAbilitySystemComponent> CachedASC; // Cached reference to the ability system component
};
    