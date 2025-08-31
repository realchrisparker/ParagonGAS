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

#include "Components/PGAS_HitboxComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UPGAS_HitboxComponent::UPGAS_HitboxComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false; // tick only when detecting
}

void UPGAS_HitboxComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPGAS_HitboxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsDetecting)
    {
        PerformSweep();
    }
}

void UPGAS_HitboxComponent::StartHitDetection(const FPGAS_AttackType& Attack)
{
    // ActiveAttack.Hitbox = Attack.Hitbox; // cache hitbox from AttackType
    ActiveAttack = Attack;          // cache full attack type
    bIsDetecting = true;

    AlreadyHitActors.Reset(); // clear at start of swing

    // Cache initial socket positions
    if (USkeletalMeshComponent* Mesh = ResolveMesh())
    {
        PrevStartLoc = Mesh->GetSocketLocation(ActiveAttack.Hitbox.StartSocket);
        PrevEndLoc = Mesh->GetSocketLocation(ActiveAttack.Hitbox.EndSocket);
    }

    SetComponentTickEnabled(true); // enable ticking
}

void UPGAS_HitboxComponent::StopHitDetection()
{
    bIsDetecting = false;
    AlreadyHitActors.Reset(); // clear at end of swing
    SetComponentTickEnabled(false); // disable ticking
}

void UPGAS_HitboxComponent::PerformSweep()
{
    // Get mesh component
    USkeletalMeshComponent* Mesh = ResolveMesh();

    // Validate mesh and sockets
    if (!Mesh || ActiveAttack.Hitbox.StartSocket == NAME_None || ActiveAttack.Hitbox.EndSocket == NAME_None)
        return;

    // Get current socket locations
    const FVector CurrStart = Mesh->GetSocketLocation(ActiveAttack.Hitbox.StartSocket);
    const FVector CurrEnd = Mesh->GetSocketLocation(ActiveAttack.Hitbox.EndSocket);

    // Sweep multiple times between previous and current positions (substepping)
    for (int32 i = 0; i < SubstepCount; i++)
    {
        const float Alpha = (i + 1) / static_cast<float>(SubstepCount);

        const FVector StepStart = FMath::Lerp(PrevStartLoc, CurrStart, Alpha);
        const FVector StepEnd = FMath::Lerp(PrevEndLoc, CurrEnd, Alpha);

        FCollisionQueryParams Params(SCENE_QUERY_STAT(PGAS_Hitbox), false, GetOwner()); // Setup collision parameters
        TArray<FHitResult> Hits; // Array to store hit results

        // Perform the sweep
        GetWorld()->SweepMultiByObjectType(
            Hits,
            StepStart,
            StepEnd,
            FQuat::Identity,
            FCollisionObjectQueryParams(ECC_Pawn),
            FCollisionShape::MakeSphere(ActiveAttack.Hitbox.Radius),
            Params
        );

        // Process hit results
        for (const FHitResult& Hit : Hits)
        {
            if (AActor* HitActor = Hit.GetActor())
            {
                if (HitActor == GetOwner()) // skip self
                    continue;

                if (AlreadyHitActors.Contains(HitActor)) // skip duplicate hits in same swing
                    continue;

                AlreadyHitActors.Add(HitActor); // mark as hit
                OnHitboxHit.Broadcast(HitActor, Hit, ActiveAttack); // Broadcast hit event

                if (ActiveAttack.Hitbox.bDebugDraw)
                {
                    DrawDebugSphere(GetWorld(), Hit.ImpactPoint, ActiveAttack.Hitbox.Radius, 12, FColor::Red, false, 0.05f);
                }
            }
        }

        // Draw sweep line
        if (ActiveAttack.Hitbox.bDebugDraw)
        {
            DrawDebugLine(GetWorld(), StepStart, StepEnd, FColor::Green, false, 0.05f, 0, 1.5f);
        }
    }

    // Update previous socket locations for next frame
    PrevStartLoc = CurrStart;
    PrevEndLoc = CurrEnd;

    // Draw socket spheres
    if (ActiveAttack.Hitbox.bDebugDraw)
    {
        DrawDebugSphere(GetWorld(), CurrStart, ActiveAttack.Hitbox.Radius, 8, FColor::Blue, false, 0.05f);
        DrawDebugSphere(GetWorld(), CurrEnd, ActiveAttack.Hitbox.Radius, 8, FColor::Blue, false, 0.05f);
    }
}
