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
 * PGAS_BlockComponent.cpp
 * Implementation of the block/guard component.
 */

#include <Components/PGAS_BlockComponent.h>
#include <Characters/Player/PGAS_PlayerCharacter.h>
#include <GAS/Effects/PGAS_GE_BlockDefenseBuff.h>
#include "GameFramework/Actor.h"
#include "Engine/World.h"

UPGAS_BlockComponent::UPGAS_BlockComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // disable ticking
}

void UPGAS_BlockComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<APGAS_PlayerCharacter>(GetOwner());
    if (OwnerCharacter)
    {
        ASC = OwnerCharacter->GetAbilitySystemComponent();
    }
}

/** Start blocking (called when block input pressed) */
void UPGAS_BlockComponent::StartBlock()
{
	if (!bIsBlocking)
	{
		bIsBlocking = true;
        UE_LOG(LogTemp, Log, TEXT("%s started blocking."), *GetOwner()->GetName());

        // Disable movement by removing movement tag
        if (OwnerCharacter && StopMovementOnBlock)
        {
            OwnerCharacter->RemoveGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove")));
        }

        if (ASC)
        {
            ASC->AbilityLocalInputPressed(static_cast<int32>(EPGAS_AbilityInputID::Block));

            // Check weapon effects for BlockDefenseBuff
            if (UPGAS_CombatCoreComponent* Core = OwnerCharacter->GetCombatCoreComponent())
            {
                UPGAS_WeaponDataAsset* WeaponData = nullptr;

                if (DominantBlockHand == EPGAS_WeaponHand::Left)
                {
                    WeaponData = Core->GetFirstAttackByHand(EPGAS_WeaponHand::Left).WeaponData;
                }
                else if (DominantBlockHand == EPGAS_WeaponHand::Right)
                {
                    WeaponData = Core->GetFirstAttackByHand(EPGAS_WeaponHand::Right).WeaponData;
                }
                else if (DominantBlockHand == EPGAS_WeaponHand::Both)
                {
                    // You could decide: return the last known attack’s weapon, 
                    // or special-case “two-handed” weapons.
                    WeaponData = Core->GetEquippedWeaponData();
                }

                // If we have weapon data
                if (WeaponData)
                {
                    for (const TSubclassOf<UGameplayEffect>& EffectClass : WeaponData->WeaponEffects)
                    {
                        if (EffectClass && EffectClass->IsChildOf(UPGAS_GE_BlockDefenseBuff::StaticClass()))
                        {
                            FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
                            FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.f, Context);
                            if (SpecHandle.IsValid())
                            {
                                static FGameplayTag BlockDefenseTag = FGameplayTag::RequestGameplayTag(FName("Combat.Blocking"));
                                SpecHandle.Data->SetSetByCallerMagnitude(BlockDefenseTag, WeaponData->BlockBuffValue);

                                BlockDefenseBuffHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                            }
                        }
                    }
                }
            }
        }

        // Notify listeners
        OnBlockStateChanged.Broadcast(bIsBlocking);
    }
}

/** Stop blocking (called when block input released) */
void UPGAS_BlockComponent::StopBlock()
{
	if (bIsBlocking)
	{
		bIsBlocking = false;
        UE_LOG(LogTemp, Log, TEXT("%s stopped blocking."), *GetOwner()->GetName());

        // Re-enable movement by restoring movement tag
        if (OwnerCharacter && StopMovementOnBlock)
        {
            OwnerCharacter->AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove")));
        }

        if (ASC)
        {
            ASC->AbilityLocalInputReleased(static_cast<int32>(EPGAS_AbilityInputID::Block));

            // Remove BlockDefenseBuff if we have one
            if (BlockDefenseBuffHandle.IsValid())
            {
                ASC->RemoveActiveGameplayEffect(BlockDefenseBuffHandle);
                BlockDefenseBuffHandle.Invalidate();
            }
        }

        // Notify listeners
        OnBlockStateChanged.Broadcast(bIsBlocking);
	}
}
