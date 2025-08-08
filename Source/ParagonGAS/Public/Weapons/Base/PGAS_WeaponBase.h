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
 * Date: 8-7-2025
 * =============================================================================
 * Base weapon actor that holds a static mesh (e.g., broadsword) and can be equipped to a character socket.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "PGAS_WeaponBase.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PARAGONGAS_API APGAS_WeaponBase : public AActor
{
    GENERATED_BODY()

public:
    APGAS_WeaponBase();

    /** Static mesh for the weapon (broadsword, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UStaticMeshComponent> WeaponMesh;

    /** Optional: gameplay tags that describe this weapon (DamageType.Slash, Weapon.Sword, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FGameplayTagContainer WeaponTags;

    /** Default socket to attach to (override per-BP if needed) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName AttachSocketName = FName("hand_r_socket");

    /** Equip to a skeletal mesh component at the weapon’s AttachSocketName */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipToMesh(USkeletalMeshComponent* TargetMesh);

    /** Detach from owner mesh (keeps world transform by default) */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Unequip();

protected:
    virtual void BeginPlay() override;
};
