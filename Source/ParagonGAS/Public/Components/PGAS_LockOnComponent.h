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
 * Date: 8-30-2025
 * =============================================================================
 * PGAS_LockOnComponent.h
 * A lightweight, Blueprint-friendly lock-on component for player/AI that:
 *  - Finds a target within radius/FOV, with LOS check.
 *  - Keeps camera/character oriented toward target (interp).
 *  - Supports switching to nearest target to the left/right.
 *  - Exposes events for UI reticles and gameplay.
 *  - Uses TObjectPtr and UE5.5+/5.6 conventions.
 */
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "PGAS_LockOnComponent.generated.h"

 // Forward declarations (avoid heavy includes in header)
class USpringArmComponent;
class UCharacterMovementComponent;
class UCameraComponent;
class APlayerController;
class APawn;
class UUserWidget;

/*
 * Delegate signatures for lock-on events.
 */

/** Fired when a target is successfully locked onto. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPGAS_OnLockedSignature, AActor*, NewTarget);
/** Fired when the lock is broken/unlocked. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPGAS_OnUnlockedSignature, AActor*, OldTarget);
/** Fired when lock target changes while still locked (e.g., switch left/right). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPGAS_OnTargetChangedSignature, AActor*, OldTarget, AActor*, NewTarget);

/**
 * Lock-on / soft-targeting component.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (PGAS),
    meta = (BlueprintSpawnableComponent, 
    DisplayName = "Lock On Component",
    Description = "A component that enables soft-locking onto targets.",
    ToolTip = "A component that enables soft-locking onto targets.")
)
class PARAGONGAS_API UPGAS_LockOnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Constructor
    UPGAS_LockOnComponent();

    /*
     * Functions
     */
    
     /** Attempt to lock onto the best candidate in front of the camera/owner. Returns true if a target was found. */
    UFUNCTION(BlueprintCallable, Category = "LockOn")
    bool TryLockOnNearest();

    /** Unlock from the current target (if any). */
    UFUNCTION(BlueprintCallable, Category = "LockOn")
    void Unlock();

    /** Switch to the next best target to the right (screen-space). Returns true if switched. */
    UFUNCTION(BlueprintCallable, Category = "LockOn")
    bool SwitchTargetRight();

    /** Switch to the next best target to the left (screen-space). Returns true if switched. */
    UFUNCTION(BlueprintCallable, Category = "LockOn")
    bool SwitchTargetLeft();

    /** Returns the current locked target (or nullptr). */
    UFUNCTION(BlueprintPure, Category = "LockOn")
    AActor* GetCurrentTarget() const { return CurrentTarget.Get(); }

    /** Is the owner currently locked onto a target? */
    UFUNCTION(BlueprintPure, Category = "LockOn")
    bool IsLockedOn() const { return CurrentTarget.IsValid(); }

    /** World-space aim location for the current (or provided) target for reticles/traces. */
    UFUNCTION(BlueprintPure, Category = "LockOn")
    FVector GetAimLocation(const AActor* TargetOverride = nullptr) const;

    /** Get screen position of current target for HUD reticle. Returns false if off-screen or no player controller. */
    UFUNCTION(BlueprintCallable, Category = "LockOn", meta = (WorldContext = "WorldContextObject"))
    bool GetTargetScreenPosition(UObject* WorldContextObject, FVector2D& OutScreenPos) const;

    /** Spawn the reticle widget when locking on. */
    void ShowReticle();

    /** Remove or hide the reticle widget when unlocking. */
    void HideReticle();

    /** Update the reticle widget’s screen position each frame. */
    void UpdateReticle(UObject* WorldContextObject);

    /*
     * Properties
     */
    
    //---------------
    // Events
    //---------------

    /** Broadcast when a target is locked. */
    UPROPERTY(BlueprintAssignable, Category = "LockOn|Events")
    FPGAS_OnLockedSignature OnLocked;

    /** Broadcast when lock is removed. */
    UPROPERTY(BlueprintAssignable, Category = "LockOn|Events")
    FPGAS_OnUnlockedSignature OnUnlocked;

    /** Broadcast when switching targets while staying locked. */
    UPROPERTY(BlueprintAssignable, Category = "LockOn|Events")
    FPGAS_OnTargetChangedSignature OnTargetChanged;

    //---------------
    // Tuning
    //---------------

    /** Actor class filter for valid targets (defaults to APGAS_EnemyCharacter). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Filter")
    TSubclassOf<AActor> TargetClassFilter;

    /** Search radius for lock-on (world units). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Filter", meta = (ClampMin = "100.0", UIMin = "100.0"))
    float LockOnRadius = 2500.f;

    /** Half-angle FOV in degrees in front of camera/owner to consider candidates (e.g., 70 => 140° cone). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Filter", meta = (ClampMin = "5.0", ClampMax = "175.0"))
    float LockOnHalfFOV = 70.f;

    /** Optional name of a socket/bone to aim at on skeletal targets (falls back gracefully). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Aim")
    FName AimSocketName = TEXT("head");

    /** Trace channel used to validate line-of-sight from camera to target. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|LOS")
    TEnumAsByte<ECollisionChannel> LineOfSightChannel = ECollisionChannel::ECC_Visibility;

    /** Draw debug helpers (candidate cone, LOS line, current target). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Debug")
    bool bDebugDraw = false;

    /** Maximum distance before lock breaks automatically. 0 = disabled. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Filter")
    float MaxLockDistance = 3000.f;

    //---------------
    // Orientation
    //---------------    

    /** While locked, smoothly rotate the controller toward the target (affects camera). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Orientation")
    bool bRotateControllerTowardTarget = true;

    /** While locked, smoothly rotate the character yaw toward the target. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Orientation")
    bool bRotateCharacterTowardTarget = true;

    /** Interp speed for controller/character yaw while locked. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Orientation", meta = (ClampMin = "0.1", UIMin = "0.1"))
    float RotationInterpSpeed = 10.f;

    //---------------
    // UI
    //---------------

    /** Widget class to display at the locked target’s aim socket (e.g. reticle). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|UI", meta=(DisplayName="LockOn Reticle"))
    TSubclassOf<UUserWidget> LockOnReticleWidgetClass;

protected:
    /*
     * Functions
     */

    // Game begins
    virtual void BeginPlay() override;
    // Game ends
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    /*
     * Functions
     */
    
     /** Rebuild the list of visible/valid candidates sorted by angle to view forward. */
    void RebuildCandidateList();

    /** Pick best candidate by smallest on-axis angle. */
    AActor* SelectBestCandidate() const;

    /** Get camera/view origin & forward. */
    bool GetViewInfo(FVector& OutOrigin, FVector& OutForward) const;

    /** Screen-space bearing signed (right positive) for switching. */
    bool GetScreenDirectionToTarget(const AActor* Target, float& OutSignedX) const;

    /** Check LOS from view to target aim location. */
    bool HasLineOfSightTo(const AActor* Target) const;

    /** Apply smooth orientation toward the current target. */
    void UpdateOrientation(float DeltaTime);

    /** Safely resolve an aim point on a target. */
    FVector ResolveAimLocation(const AActor* Target) const;

    /** Clear lock, restore movement settings. */
    void InternalUnlock(bool bBroadcast);

    /** Handle death of our current target. Only valid if we are locked on. */
    UFUNCTION()
    void HandleTargetDeath();

    /*
     * Properties
     */
    
    /** Current locked target (not replicated; lock-on is client-local by design). */
    UPROPERTY(Transient)
    TWeakObjectPtr<AActor> CurrentTarget;

    /** Working cache of viable candidates (sorted). */
    UPROPERTY(Transient)
    TArray<TWeakObjectPtr<AActor>> CandidateCache;

    /** Cached components/settings to restore on unlock. */
    UPROPERTY(Transient)
    TObjectPtr<UCharacterMovementComponent> CachedMoveComp = nullptr;

    bool bSavedOrientRotationToMovement = false;
    bool bSavedUseControllerDesiredRotation = false;

    /** Active reticle widget instance. */
    UPROPERTY(Transient)
    TObjectPtr<UUserWidget> ActiveReticleWidget;
};
