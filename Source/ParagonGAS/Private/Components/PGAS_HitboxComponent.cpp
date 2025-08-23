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
 * Implementation for socket-driven melee hitbox component.
 */

#include "Components/PGAS_HitboxComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GenericTeamAgentInterface.h"

UPGAS_HitboxComponent::UPGAS_HitboxComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    // Default to tracing Pawns if no CollisionProfileName provided.
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void UPGAS_HitboxComponent::BeginPlay()
{
    Super::BeginPlay();
    CacheOwnerASC();
}

void UPGAS_HitboxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (ActiveSets.Num() == 0)
    {
        return;
    }

    TimeSinceLastSweep += DeltaTime;
    if (SweepInterval > 0.f && TimeSinceLastSweep < SweepInterval)
    {
        return;
    }
    TimeSinceLastSweep = 0.0;

    for (const FPGAS_HitboxSet& Set : HitboxSets)
    {
        if (!ActiveSets.Contains(Set.SetName))
        {
            continue;
        }

        USkeletalMeshComponent* Mesh = ResolveMeshForSet(Set);
        if (!Mesh) { continue; }

        EnsurePrevTransforms(Set, Mesh);
        SweepSet(Set);
        UpdatePrevTransforms(Set, Mesh);
    }
}

void UPGAS_HitboxComponent::SetHitboxSets(const TArray<FPGAS_HitboxSet>& InSets)
{
    HitboxSets = InSets;
    PrevSocketTransforms.Empty();
}

void UPGAS_HitboxComponent::StartHitDetection(FName SetName, FGameplayTag InAttackTag)
{
    if (SetName.IsNone())
    {
        // Activate all sets if none specified.
        for (const FPGAS_HitboxSet& S : HitboxSets)
        {
            ActiveSets.Add(S.SetName);
            AlreadyHitPerSet.FindOrAdd(S.SetName).Reset();
        }
    }
    else
    {
        ActiveSets.Add(SetName);
        AlreadyHitPerSet.FindOrAdd(SetName).Reset();
    }
    CurrentAttackTag = InAttackTag;
}

void UPGAS_HitboxComponent::StopHitDetection(FName SetName)
{
    if (SetName.IsNone())
    {
        ActiveSets.Reset();
    }
    else
    {
        ActiveSets.Remove(SetName);
    }
}

void UPGAS_HitboxComponent::ResetAlreadyHit(FName SetName)
{
    if (SetName.IsNone())
    {
        for (auto& Kvp : AlreadyHitPerSet)
        {
            Kvp.Value.Reset();
        }
    }
    else
    {
        if (TSet<TWeakObjectPtr<AActor>>* Set = AlreadyHitPerSet.Find(SetName))
        {
            Set->Reset();
        }
    }
}

void UPGAS_HitboxComponent::AddIgnoredActor(AActor* ActorToIgnore)
{
    if (IsValid(ActorToIgnore))
    {
        IgnoredActors.Add(ActorToIgnore);
    }
}

void UPGAS_HitboxComponent::RemoveIgnoredActor(AActor* ActorToUnignore)
{
    if (IsValid(ActorToUnignore))
    {
        IgnoredActors.Remove(ActorToUnignore);
    }
}

void UPGAS_HitboxComponent::SetDebugDraw(bool bEnable)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    SetComponentTickEnabled(true);
#endif
    // We piggyback on DrawDebugHelpers calls, so just rely on Collision Debug cvars/your own toggles where you use DrawDebug.
    // If you want a full property, you can add a UPROPERTY bool and gate the DrawDebug calls below.
}

bool UPGAS_HitboxComponent::IsSetActive(FName SetName) const
{
    return ActiveSets.Contains(SetName);
}

USkeletalMeshComponent* UPGAS_HitboxComponent::ResolveMeshForSet(const FPGAS_HitboxSet& Set) const
{
    if (Set.Mesh)
    {
        return Set.Mesh.Get();
    }
    // Fallback: first skeletal mesh on owner
    if (const AActor* OwnerActor = GetOwner())
    {
        return OwnerActor->FindComponentByClass<USkeletalMeshComponent>();
    }
    return nullptr;
}

void UPGAS_HitboxComponent::CacheOwnerASC()
{
    if (AActor* OwnerActor = GetOwner())
    {
        OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
    }
}

bool UPGAS_HitboxComponent::AreFriendly(AActor* A, AActor* B) const
{
    if (!bIgnoreSameTeam || !A || !B) return false;

    const IGenericTeamAgentInterface* TA = Cast<const IGenericTeamAgentInterface>(A);
    const IGenericTeamAgentInterface* TB = Cast<const IGenericTeamAgentInterface>(B);
    if (!TA || !TB) return false;

    return TA->GetGenericTeamId() == TB->GetGenericTeamId();
}

void UPGAS_HitboxComponent::EnsurePrevTransforms(const FPGAS_HitboxSet& Set, USkeletalMeshComponent* Mesh)
{
    TMap<FName, FTransform>& Cache = PrevSocketTransforms.FindOrAdd(Set.SetName);

    for (const FPGAS_BladeTrace& Def : Set.Traces)
    {
        if (Def.StartSocket != NAME_None && !Cache.Contains(Def.StartSocket))
        {
            Cache.Add(Def.StartSocket, Mesh->GetSocketTransform(Def.StartSocket));
        }
        if (Def.EndSocket != NAME_None && !Cache.Contains(Def.EndSocket))
        {
            Cache.Add(Def.EndSocket, Mesh->GetSocketTransform(Def.EndSocket));
        }
    }
}

void UPGAS_HitboxComponent::UpdatePrevTransforms(const FPGAS_HitboxSet& Set, USkeletalMeshComponent* Mesh)
{
    if (TMap<FName, FTransform>* Cache = PrevSocketTransforms.Find(Set.SetName))
    {
        for (const FPGAS_BladeTrace& Def : Set.Traces)
        {
            if (Def.StartSocket != NAME_None)
            {
                (*Cache).FindOrAdd(Def.StartSocket) = Mesh->GetSocketTransform(Def.StartSocket);
            }
            if (Def.EndSocket != NAME_None)
            {
                (*Cache).FindOrAdd(Def.EndSocket) = Mesh->GetSocketTransform(Def.EndSocket);
            }
        }
    }
}

FCollisionQueryParams UPGAS_HitboxComponent::MakeQueryParams() const
{
    FCollisionQueryParams Params(SCENE_QUERY_STAT(PGAS_Hitbox), false, GetOwner());
    Params.bReturnPhysicalMaterial = false;
    Params.bTraceComplex = false;

    // Add ignored actors
    for (const TWeakObjectPtr<AActor>& WeakA : IgnoredActors)
    {
        if (AActor* A = WeakA.Get())
        {
            Params.AddIgnoredActor(A);
        }
    }
    // Always ignore self
    if (AActor* OwnerActor = GetOwner())
    {
        Params.AddIgnoredActor(OwnerActor);
    }
    return Params;
}

FCollisionObjectQueryParams UPGAS_HitboxComponent::MakeObjectParams() const
{
    if (CollisionProfileName != NAME_None)
    {
        // Using collision profile via SweepByChannel isn’t supported directly here; keeping object params when profile not set.
    }
    FCollisionObjectQueryParams ObjParams;
    for (auto ObjType : ObjectTypes)
    {
        ObjParams.AddObjectTypesToQuery(UEngineTypes::ConvertToCollisionChannel(ObjType));
    }
    return ObjParams;
}

void UPGAS_HitboxComponent::SweepSet(const FPGAS_HitboxSet& Set)
{
    USkeletalMeshComponent* Mesh = ResolveMeshForSet(Set);
    if (!Mesh) return;

    const TMap<FName, FTransform>* Prev = PrevSocketTransforms.Find(Set.SetName);
    if (!Prev) return;

    for (const FPGAS_BladeTrace& Def : Set.Traces)
    {
        if (Def.StartSocket == NAME_None || Def.EndSocket == NAME_None) continue;

        const FTransform CurrStart = Mesh->GetSocketTransform(Def.StartSocket);
        const FTransform CurrEnd = Mesh->GetSocketTransform(Def.EndSocket);

        const FTransform* PrevStartPtr = Prev->Find(Def.StartSocket);
        const FTransform* PrevEndPtr = Prev->Find(Def.EndSocket);
        if (!PrevStartPtr || !PrevEndPtr) continue;

        const FVector PrevStart = PrevStartPtr->GetLocation();
        const FVector PrevEnd = PrevEndPtr->GetLocation();
        const FVector CurrStartLoc = CurrStart.GetLocation();
        const FVector CurrEndLoc = CurrEnd.GetLocation();

        // Cover motion between frames and across the blade span
        SweepBetween(PrevStart, CurrStartLoc, Def, Set.SetName);
        SweepBetween(PrevEnd, CurrEndLoc, Def, Set.SetName);
        SweepBetween(PrevStart, PrevEnd, Def, Set.SetName);
        SweepBetween(CurrStartLoc, CurrEndLoc, Def, Set.SetName);
    }
}

void UPGAS_HitboxComponent::SweepBetween(const FVector& From, const FVector& To, const FPGAS_BladeTrace& Def, FName SetName)
{
    UWorld* World = GetWorld();
    if (!World) return;

    const FVector Dir = (To - From);
    const float Dist = Dir.Size();
    const FVector NormDir = Dist > KINDA_SMALL_NUMBER ? Dir / Dist : FVector::ForwardVector;

    FCollisionQueryParams Params = MakeQueryParams();
    FCollisionObjectQueryParams ObjParams = MakeObjectParams();

    TArray<FHitResult> Hits;

    switch (Def.Shape)
    {
        case EPGAS_HitboxShape::Sphere:
            {
                const FCollisionShape Shape = FCollisionShape::MakeSphere(Def.Radius);
                World->SweepMultiByObjectType(Hits, From, To, FQuat::Identity, ObjParams, Shape, Params);
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
                // Debug
                // DrawDebugLine(World, From, To, FColor::Red, false, 0.03f, 0, 1.0f);
                // DrawDebugSphere(World, To, Def.Radius, 12, FColor::Red, false, 0.03f);
#endif
                break;
            }
        case EPGAS_HitboxShape::Capsule:
            {
                // Capsule oriented along sweep direction.
                const FQuat Rot = FRotationMatrix::MakeFromZ(NormDir).ToQuat();
                const FCollisionShape Shape = FCollisionShape::MakeCapsule(Def.Radius, Def.CapsuleHalfHeight);
                World->SweepMultiByObjectType(Hits, From, To, Rot, ObjParams, Shape, Params);
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
                // DrawDebugCapsule(World, To, Def.CapsuleHalfHeight, Def.Radius, Rot, FColor::Red, false, 0.03f);
#endif
                break;
            }
        default:
            break;
    }

    if (Hits.Num() == 0) return;

    TSet<TWeakObjectPtr<AActor>>& AlreadyHit = AlreadyHitPerSet.FindOrAdd(SetName);

    for (const FHitResult& H : Hits)
    {
        AActor* HitActor = H.GetActor();
        if (!IsValid(HitActor)) continue;

        if (IgnoredActors.Contains(HitActor)) continue;
        if (AreFriendly(GetOwner(), HitActor)) continue;

        if (AlreadyHit.Contains(HitActor))
        {
            continue; // one hit per window per actor
        }

        AlreadyHit.Add(HitActor);
        HandleHit(HitActor, H, SetName);
    }
}

void UPGAS_HitboxComponent::HandleHit(AActor* HitActor, const FHitResult& Hit, FName SetName)
{
    // Broadcast first so abilities/blueprints can react (apply impulses, play VFX/SFX, etc.)
    OnHitboxHit.Broadcast(HitActor, Hit, SetName, CurrentAttackTag);

    // Optional: apply a GameplayEffect (damage, etc.)
    ApplyGameplayEffectIfConfigured(HitActor, Hit);
}

void UPGAS_HitboxComponent::ApplyGameplayEffectIfConfigured(AActor* TargetActor, const FHitResult& /*Hit*/) const
{
    if (!DamageEffectClass || !OwnerASC || !IsValid(TargetActor))
    {
        return;
    }

    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (!TargetASC) return;

    FGameplayEffectContextHandle Ctx = OwnerASC->MakeEffectContext();
    Ctx.AddSourceObject(this);

    FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(DamageEffectClass, /*Level*/1.f, Ctx);
    if (!SpecHandle.IsValid()) return;

    if (SetByCallerMagnitudeTag.IsValid())
    {
        SpecHandle.Data.Get()->SetSetByCallerMagnitude(SetByCallerMagnitudeTag, EffectMagnitude);
    }

    OwnerASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}
