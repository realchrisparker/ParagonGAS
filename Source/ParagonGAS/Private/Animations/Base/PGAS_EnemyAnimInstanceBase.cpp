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
 * Date: 8-8-2025
 * =============================================================================
 * PGAS_AnimInstanceBase.h
 * Base class for all Enemy Animation Instances in ParagonGAS.
 * This class serves as a foundation for custom animation instances, providing a common interface and functionality.
*/


#include "Animations/Base/PGAS_EnemyAnimInstanceBase.h"

// Constructor
UPGAS_EnemyAnimInstanceBase::UPGAS_EnemyAnimInstanceBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bUseMultiThreadedAnimationUpdate = true; // Enable multi-threaded animation updates
    RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly; // Set root motion mode to only from montages
}

// Called when the animation instance is destorying
void UPGAS_EnemyAnimInstanceBase::BeginDestroy()
{
    Super::BeginDestroy();

    try
    {
        CachedMovementComponent = nullptr; // Clear cached movement component pointer
    }
    catch (...)
    {
    }
}

// Initialize the animation instance
void UPGAS_EnemyAnimInstanceBase::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Initialize any variables or perform setup here
    if (APawn* Pawn = TryGetPawnOwner())
    {
        OwningPawn = Pawn; // Cache the owning pawn
        CachedMovementComponent = Cast<UCharacterMovementComponent>(OwningPawn->GetMovementComponent()); // Cache the movement component        
    }
}

// Called when the animation instance updates
void UPGAS_EnemyAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (OwningPawn)
    {
        // Get actor rotation
        OwningPawnActorRotation = OwningPawn->GetActorRotation();

        // Get unarmed state
        APGAS_EnemyCharacter* Character = Cast<APGAS_EnemyCharacter>(OwningPawn);
        if (Character)
        {
            IsArmed = Character->IsWeaponEquipped();
        }

        // Get rotation
        Rotation = OwningPawn->GetActorRotation();
    }
}

// Called when the animation instance updates thread safe
void UPGAS_EnemyAnimInstanceBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

    // Update animation properties based on the movement component
    if (CachedMovementComponent || OwningPawn)
    {
        // Get is in air state
        IsInAir = CachedMovementComponent->IsFalling();

        // Get velocity
        Velocity = CachedMovementComponent->Velocity;

        // Get speed and movement state
        Speed = CachedMovementComponent->Velocity.Size();
        IsMoving = Speed > 0.0f;

        // Calculate direction using rotation    
        Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, OwningPawnActorRotation); // returns -180..180 (0=fwd, +90=right, -90=left)

        // Roll & Pitch, normalize to 0.0001
        Roll = FMath::Clamp(Rotation.Roll, -0.0001f, 0.0001f);
        Pitch = FMath::Clamp(Rotation.Pitch, -0.0001f, 0.0001f);
    }
}