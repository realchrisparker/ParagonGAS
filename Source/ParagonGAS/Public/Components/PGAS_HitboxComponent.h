/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * Author: Christopher D. Parker
 * Date: 8-28-2025
 * =============================================================================
 * Simplified Hitbox Component (v1) – performs a basic socket-driven sphere trace.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/PGAS_WeaponHand.h"
#include <Structs/PGAS_AttackType.h>
#include "PGAS_HitboxComponent.generated.h"

/*
 * Declarations
 */

class UPGAS_WeaponDataAsset;


/*
 * Delegates
 */

// Delegate for hit events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FPGAS_OnSimpleHitSignature,
    AActor*, HitActor,
    const FHitResult&, HitResult,
    FPGAS_AttackType, AttackType
);

/**
 * Simple socket-driven hitbox (StartSocket → EndSocket).
 * Tick-based sphere sweep while detection is active.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (PGAS),
    meta = (BlueprintSpawnableComponent, DisplayName = "Simple Hitbox Component"))
    class PARAGONGAS_API UPGAS_HitboxComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPGAS_HitboxComponent();

    /*
     * Functions
     */
    
    /** Start hit detection (begins ticking sweeps). */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Hitbox")
    void StartHitDetection(const FPGAS_AttackType& Attack);

    /** Stop hit detection (stops ticking sweeps). */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Hitbox")
    void StopHitDetection();

    /** Is currently detecting hits? */
    UFUNCTION(BlueprintPure, Category = "PGAS|Hitbox")
    bool IsDetecting() const { return bIsDetecting; }

    /**
     * Clear the list of already hit actors for the current swing.
     * Useful to allow multi-hit within a combo.
     */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Hitbox")
    void ResetAlreadyHit()
    {
        AlreadyHitActors.Reset();
    }

    /*
     * Properties
     */

    /** Broadcast when a hit is registered. */
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Hitbox")
    FPGAS_OnSimpleHitSignature OnHitboxHit;

protected:
    /*
     * Functions
     */

    /** Called when the game starts */
    virtual void BeginPlay() override;

    /** Called every frame */
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    /*
     * Functions
     */

    USkeletalMeshComponent* ResolveMesh() const
    {
        if (const AActor* Owner = GetOwner())
        {
            return Owner->FindComponentByClass<USkeletalMeshComponent>();
        }

        return nullptr;
    }

    void PerformSweep();

    /*
     * Properties
     */

    // Is currently detecting hits?
    bool bIsDetecting = false;

    // Cached from AttackType when StartHitDetection is called
    FPGAS_AttackType ActiveAttack;

    // Previous frame socket positions
    FVector PrevStartLoc = FVector::ZeroVector;
    FVector PrevEndLoc = FVector::ZeroVector;

    // Number of substeps to interpolate between frames
    UPROPERTY(EditAnywhere, Category = "PGAS|Hitbox")
    int32 SubstepCount = 2; // 2–4 is usually plenty

    // Prevent hitting same actor multiple times in one swing
    TSet<TWeakObjectPtr<AActor>> AlreadyHitActors;
};
