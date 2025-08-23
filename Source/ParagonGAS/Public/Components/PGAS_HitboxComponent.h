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
 * Date: 8-19-2025
 * =============================================================================
 * Hitbox component that performs socket-driven weapon traces for melee attacks.
 * Supports left/right (or any named) hitbox sets, once-per-window hit filtering,
 * friendly-fire filtering, optional GameplayEffect application, and debug draw.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "PGAS_HitboxComponent.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class USkeletalMeshComponent;

UENUM(BlueprintType)
enum class EPGAS_HitboxShape : uint8
{
    Sphere  UMETA(DisplayName = "Sphere"),
    Capsule UMETA(DisplayName = "Capsule"),
    // Box can be added later; sphere/capsule cover most melee cases efficiently.
};

USTRUCT(BlueprintType)
struct PARAGONGAS_API FPGAS_BladeTrace
{
    GENERATED_BODY()

    /** Start socket (e.g., "Blade_Base"). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    FName StartSocket = NAME_None;

    /** End socket (e.g., "Blade_Tip"). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    FName EndSocket = NAME_None;

    /** Trace radius used for sphere/capsule sweeps along the blade. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox", meta = (ClampMin = "0.0"))
    float Radius = 5.f;

    /** For Capsule shape only (half-height along the sweep axis). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox", meta = (ClampMin = "0.0"))
    float CapsuleHalfHeight = 0.f;

    /** Shape used when sweeping between sockets. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    EPGAS_HitboxShape Shape = EPGAS_HitboxShape::Sphere;
};

USTRUCT(BlueprintType)
struct PARAGONGAS_API FPGAS_HitboxSet
{
    GENERATED_BODY()

    /** A name for this set (e.g., "LeftHand", "RightHand", "Staff"). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    FName SetName = TEXT("Default");

    /** Mesh to read sockets from (defaults to Owner's first skeletal mesh if not set). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    TObjectPtr<USkeletalMeshComponent> Mesh = nullptr;

    /** One or more blade traces that will be evaluated each tick while active. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    TArray<FPGAS_BladeTrace> Traces;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FPGAS_OnHitboxHitSignature, AActor*, HitActor, const FHitResult&, HitResult, FName, HitboxSetName, FGameplayTag, AttackTag);

/**
 * Socket-driven melee hitbox component.
 * Call StartHitDetection/StopHitDetection from Ability (or Anim Notifies).
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (PGAS), meta = (BlueprintSpawnableComponent, DisplayName = "PGAS Hitbox Component"))
class PARAGONGAS_API UPGAS_HitboxComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPGAS_HitboxComponent();

    /** Assign/override hitbox sets at runtime (alternatively, set in defaults). */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Hitbox")
    void SetHitboxSets(const TArray<FPGAS_HitboxSet>& InSets);

    /** Begin detecting hits for a specific set (e.g., "LeftHand") with an optional AttackTag. */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Hitbox")
    void StartHitDetection(FName SetName, FGameplayTag InAttackTag);

    /** Stop detecting hits for a specific set (or all if SetName is NAME_None). */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Hitbox")
    void StopHitDetection(FName SetName = NAME_None);

    /** Clear the list of actors already hit in the current window for a set (or all). */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Hitbox")
    void ResetAlreadyHit(FName SetName = NAME_None);

    /** Add actor to ignore list (self is always ignored). */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Hitbox")
    void AddIgnoredActor(AActor* ActorToIgnore);

    /** Remove actor from ignore list. */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Hitbox")
    void RemoveIgnoredActor(AActor* ActorToUnignore);

    /** Enable/disable debug draw lines and impacts. */
    UFUNCTION(BlueprintCallable, Category = "PGAS|Hitbox")
    void SetDebugDraw(bool bEnable);

    /** True if a given set is currently tracing. */
    UFUNCTION(BlueprintPure, Category = "PGAS|Hitbox")
    bool IsSetActive(FName SetName) const;

    /** Blueprint event when we register a new hit (not repeated for the same actor within a window). */
    UPROPERTY(BlueprintAssignable, Category = "PGAS|Hitbox")
    FPGAS_OnHitboxHitSignature OnHitboxHit;

    /** Optional GAS: apply this GameplayEffect to targets on hit (leave null to just broadcast). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Hitbox|GAS")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    /** If using DamageEffectClass with SetByCaller, this tag will be used for the magnitude below. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Hitbox|GAS")
    FGameplayTag SetByCallerMagnitudeTag;

    /** Magnitude used when applying the effect (e.g., damage). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Hitbox|GAS")
    float EffectMagnitude = 0.f;

    /** Object types to trace against (default: Pawns). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Hitbox|Trace")
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

    /** Collision profile name used for sweeps (if set). Otherwise uses ObjectTypes. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Hitbox|Trace")
    FName CollisionProfileName = NAME_None;

    /** Minimum interval between sweeps (seconds). 0 = every tick. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Hitbox|Perf", meta = (ClampMin = "0.0"))
    float SweepInterval = 0.f;

    /** Ignore same-team targets (requires actors implementing IGenericTeamAgentInterface). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PGAS|Hitbox|Filter")
    bool bIgnoreSameTeam = true;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    /** Runtime copy of configured sets. */
    UPROPERTY(EditAnywhere, Category = "PGAS|Hitbox", meta = (AllowPrivateAccess = "true"))
    TArray<FPGAS_HitboxSet> HitboxSets;

    /** Active sets by name. */
    UPROPERTY(Transient)
    TSet<FName> ActiveSets;

    /** Attack tag set when StartHitDetection is called. */
    UPROPERTY(Transient)
    FGameplayTag CurrentAttackTag;

    /** Track already-hit actors per set to prevent multi-hits in same window. */
    TMap<FName, TSet<TWeakObjectPtr<AActor>>> AlreadyHitPerSet;

    /** Previous socket transforms per set/socket to sweep between frames. */
    TMap<FName, TMap<FName, FTransform>> PrevSocketTransforms;

    /** Actors to ignore. */
    TSet<TWeakObjectPtr<AActor>> IgnoredActors;

    /** Cached owner’s AbilitySystem (optional but convenient). */
    UPROPERTY(Transient)
    TObjectPtr<UAbilitySystemComponent> OwnerASC = nullptr;

    /** Last sweep time accumulator. */
    double TimeSinceLastSweep = 0.0;

    /** Internal helpers */
    USkeletalMeshComponent* ResolveMeshForSet(const FPGAS_HitboxSet& Set) const;
    void CacheOwnerASC();
    bool AreFriendly(AActor* A, AActor* B) const;
    void EnsurePrevTransforms(const FPGAS_HitboxSet& Set, USkeletalMeshComponent* Mesh);
    void UpdatePrevTransforms(const FPGAS_HitboxSet& Set, USkeletalMeshComponent* Mesh);

    void SweepSet(const FPGAS_HitboxSet& Set);
    void SweepBetween(const FVector& From, const FVector& To, const FPGAS_BladeTrace& Def, FName SetName);

    void HandleHit(AActor* HitActor, const FHitResult& Hit, FName SetName);
    void ApplyGameplayEffectIfConfigured(AActor* TargetActor, const FHitResult& Hit) const;

    FCollisionQueryParams MakeQueryParams() const;
    FCollisionObjectQueryParams MakeObjectParams() const;
};
