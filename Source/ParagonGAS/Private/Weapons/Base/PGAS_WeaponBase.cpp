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

#include "Weapons/Base/PGAS_WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

APGAS_WeaponBase::APGAS_WeaponBase()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);

    // Usually weapons don’t need physics while equipped
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponMesh->SetGenerateOverlapEvents(false);
    WeaponMesh->SetCanEverAffectNavigation(false);

    // Optional authoring helpers (created at runtime so they exist by default in BP)
    TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
    TraceStart->SetupAttachment(WeaponMesh);

    TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
    TraceEnd->SetupAttachment(WeaponMesh);
}

void APGAS_WeaponBase::BeginPlay()
{
    Super::BeginPlay();
}

void APGAS_WeaponBase::EquipToMesh(USkeletalMeshComponent* TargetMesh)
{
    if (!TargetMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("APGAS_WeaponBase::EquipToMesh - TargetMesh is null"));
        return;
    }

    // Determine the equipped socket name based on the inventory slot (Can be one of the hands, other part of the body, or in the stash)
    switch (InventorySlot)
    {
        case EPGASWeaponInventorySlot::LeftHand:
            EquipedSocketName = InventorySocketLeftHandName;
            break;
        case EPGASWeaponInventorySlot::RightHand:
            EquipedSocketName = InventorySocketRightHandName;
            break;
        case EPGASWeaponInventorySlot::BackLeft:
            EquipedSocketName = InventorySocketBackLeftName;
            break;
        case EPGASWeaponInventorySlot::BackRight:
            EquipedSocketName = InventorySocketBackRightName;
            break;
        case EPGASWeaponInventorySlot::HipLeft:
            EquipedSocketName = InventorySocketHipLeftName;
            break;
        case EPGASWeaponInventorySlot::HipRight:
            EquipedSocketName = InventorySocketHipRightName;
            break;
        case EPGASWeaponInventorySlot::ThighLeft:
            EquipedSocketName = InventorySocketThighLeftName;
            break;
        case EPGASWeaponInventorySlot::ThighRight:
            EquipedSocketName = InventorySocketThighRightName;
            break;
        case EPGASWeaponInventorySlot::InStash:
            // Handle stash case if needed
            break;
    }

    FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
    AttachToComponent(TargetMesh, AttachRules, EquipedSocketName);
}

void APGAS_WeaponBase::Unequip()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

bool APGAS_WeaponBase::GetBladeTracePoints(FVector& OutStart, FVector& OutEnd) const
{
    // Try Scene Components first if requested
    if (BladeTraceMode == EPGASBladeTraceMode::UseSceneComponents)
    {
        if (TraceStart && TraceEnd)
        {
            OutStart = TraceStart->GetComponentLocation();
            OutEnd = TraceEnd->GetComponentLocation();
            return true;
        }
        // Soft fallback to sockets if scene comps aren’t set
    }

    // Sockets on the static mesh
    if (WeaponMesh)
    {
        const bool bHasStart = WeaponMesh->DoesSocketExist(StartSocketName);
        const bool bHasEnd = WeaponMesh->DoesSocketExist(EndSocketName);

        if (bHasStart && bHasEnd)
        {
            OutStart = WeaponMesh->GetSocketLocation(StartSocketName);
            OutEnd = WeaponMesh->GetSocketLocation(EndSocketName);
            return true;
        }
    }

    // Final fallback: approximate using mesh bounds along local X (tip = +X)
    if (WeaponMesh)
    {
        const FBoxSphereBounds B = WeaponMesh->Bounds;
        const FVector X = WeaponMesh->GetComponentTransform().GetUnitAxis(EAxis::X);
        const FVector Center = B.Origin;
        const float HalfX = B.BoxExtent.X;
        OutStart = Center - X * HalfX;
        OutEnd = Center + X * HalfX;
        return true;
    }

    return false;
}

bool APGAS_WeaponBase::SphereTraceBlade(
    float Radius,
    const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
    const TArray<AActor*>& ActorsToIgnore,
    TArray<FHitResult>& OutHits,
    bool bDebug) const
{
    UWorld* World = GetWorld();
    if (!World) return false;

    FVector Start, End;
    if (!GetBladeTracePoints(Start, End))
        return false;

    EDrawDebugTrace::Type DebugDraw = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

    return UKismetSystemLibrary::SphereTraceMultiForObjects(
        World, Start, End, Radius, ObjectTypes, false, ActorsToIgnore, DebugDraw, OutHits, true
    );
}