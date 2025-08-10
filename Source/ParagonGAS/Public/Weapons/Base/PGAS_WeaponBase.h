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

 /**
  * Enum for weapon inventory slots
  */
UENUM(BlueprintType, meta=(DisplayName = "Weapon Inventory Slot"))
enum class EPGASWeaponInventorySlot : uint8
{
    BackLeft UMETA(DisplayName = "Back Left"), //
    BackRight UMETA(DisplayName = "Back Right"), //
    HipLeft UMETA(DisplayName = "Hip Left"), //
    HipRight UMETA(DisplayName = "Hip Right"), //
    HipLeft UMETA(DisplayName = "Hip Left"), //
    HipRight UMETA(DisplayName = "Hip Right"), //
    InStash UMETA(DisplayName = "In Stash") //
};

 /**
  * Enum for weapon trace mode
  */
UENUM(BlueprintType, meta=(DisplayName = "Blade Trace Mode"))
enum class EPGASBladeTraceMode : uint8
{
    UseSceneComponents UMETA(DisplayName = "Use Scene Components"),
    UseSockets        UMETA(DisplayName = "Use StaticMesh Sockets")
};

/**
 * Base class for all weapon types.
 */
UCLASS(Blueprintable, BlueprintType)
class PARAGONGAS_API APGAS_WeaponBase : public AActor
{
    GENERATED_BODY()

public:
    /*
    * Functions
    */

    // Default constructor
    APGAS_WeaponBase();

    /** Equip to a skeletal mesh component at the weapon’s EquipedSocketName */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipToMesh(USkeletalMeshComponent* TargetMesh);

    /** Detach from owner mesh (keeps world transform by default) */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Unequip();

    /** Returns world-space points for blade trace. Falls back safely if one option is unset. */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Trace")
    bool GetBladeTracePoints(FVector& OutStart, FVector& OutEnd) const;

    /** Convenience: does a SphereTraceMulti along the blade and returns hits */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Trace")
    bool SphereTraceBlade(float Radius,
        const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
        const TArray<AActor*>& ActorsToIgnore,
        TArray<FHitResult>& OutHits,
        bool bDebug = false) const;

    /*
    * Properties
    */
    
    /** Static mesh for the weapon (broadsword, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UStaticMeshComponent> WeaponMesh;

    /** Optional: gameplay tags that describe this weapon (DamageType.Slash, Weapon.Sword, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FGameplayTagContainer WeaponTags;

    /** Default socket to attach to (override per-BP if needed) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName EquipedSocketName = FName("hand_r_socket");

    /** Default socket to attach to (override per-BP if needed) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName InventorySocketName = FName("weapon_inventory1");

    /** How to provide blade endpoints for tracing */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Trace")
    EPGASBladeTraceMode BladeTraceMode = EPGASBladeTraceMode::UseSceneComponents;
    
    /** Scene markers (non-visual). Place these at the guard/tip in the BP. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Trace",
        meta = (EditCondition = "BladeTraceMode==EPGASBladeTraceMode::UseSceneComponents",
            DisplayName = "Trace Start",
            ToolTip = "Start of the weapon's blade for tracing")
    )
    TObjectPtr<USceneComponent> TraceStart;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Trace",
        meta = (EditCondition = "BladeTraceMode==EPGASBladeTraceMode::UseSceneComponents",
            DisplayName = "Trace End",
            ToolTip = "End of the weapon's blade for tracing")
    )
    TObjectPtr<USceneComponent> TraceEnd;

    /** Socket names (on the Static Mesh asset) if you prefer sockets */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Trace",
        meta = (EditCondition = "BladeTraceMode==EPGASBladeTraceMode::UseSockets",
            DisplayName = "Start Socket",
            ToolTip = "Socket at the start of the weapon's blade for tracing")
    )
    FName StartSocketName = FName("blade_start");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Trace",
        meta = (EditCondition = "BladeTraceMode==EPGASBladeTraceMode::UseSockets",
            DisplayName = "End Socket",
            ToolTip = "Socket at the end of the weapon's blade for tracing")
    )
    FName EndSocketName = FName("blade_end");

protected:
    virtual void BeginPlay() override;
};
