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

APGAS_WeaponBase::APGAS_WeaponBase()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);

    // Usually weapons don’t need physics while equipped
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponMesh->SetGenerateOverlapEvents(false);
    WeaponMesh->SetCanEverAffectNavigation(false);
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

    FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
    AttachToComponent(TargetMesh, AttachRules, AttachSocketName);
}

void APGAS_WeaponBase::Unequip()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
