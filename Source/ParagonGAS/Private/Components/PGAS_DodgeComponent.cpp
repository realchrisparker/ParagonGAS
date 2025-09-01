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
 * PGAS_DodgeComponent.cpp
 * This file implements the Dodge Component for handling dodge functionality.
 */

#include <Components/PGAS_DodgeComponent.h>
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


 // Constructor
UPGAS_DodgeComponent::UPGAS_DodgeComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // Disable ticking
}

// Called when the game starts
void UPGAS_DodgeComponent::BeginPlay()
{
    Super::BeginPlay();
}

/** Get the first dodge that matches a given category and direction. */
FPGAS_DodgeType UPGAS_DodgeComponent::GetDodgeByCategoryAndDirection(EPGAS_DodgeCategory Category, EPGAS_DodgeDirection Direction) const
{
	for (const FPGAS_DodgeType& Dodge : Dodges)
	{
		if (Dodge.DodgeCategory == Category && Dodge.Direction == Direction)
		{
			return Dodge;
		}
	}
	return FPGAS_DodgeType(); // return empty if none found
}

/** Get all dodges that match a given category (Dodge vs Roll). */
TArray<FPGAS_DodgeType> UPGAS_DodgeComponent::GetAllDodgesByCategory(EPGAS_DodgeCategory Category) const
{
	TArray<FPGAS_DodgeType> Results;
	for (const FPGAS_DodgeType& Dodge : Dodges)
	{
		if (Dodge.DodgeCategory == Category)
		{
			Results.Add(Dodge);
		}
	}
	return Results;
}

/**
 * Perform a dodge using the specified DodgeType.
 * Wraps the DodgeType into an object and sends it to the Ability System.
 */
void UPGAS_DodgeComponent::PerformDodge(const FPGAS_DodgeType& Dodge)
{
    UE_LOG(LogTemp, Warning, TEXT("Performing Dodge: %s"), *Dodge.AbilityTag.GetTagName().ToString());
    if (!GetOwner()) return;
    if (!Dodge.AbilityTag.IsValid()) return; // Must have a valid ability tag to perform dodge
    if (!Dodge.Montage) return; // Must have a valid montage to perform dodge

    // Wrap struct into UObject so GAS can carry it
    UPGAS_DodgeTypeObject* Wrapper = NewObject<UPGAS_DodgeTypeObject>(GetOwner());
    Wrapper->DodgeType = Dodge;

    FGameplayEventData EventData;
    EventData.EventTag = Dodge.AbilityTag;
    EventData.Instigator = GetOwner();
    EventData.OptionalObject = Wrapper;

    UE_LOG(LogTemp, Warning, TEXT("Sending Gameplay Event to Actor: %s with Tag: %s"), *GetOwner()->GetName(), *Dodge.AbilityTag.GetTagName().ToString());

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), Dodge.AbilityTag, EventData);
}