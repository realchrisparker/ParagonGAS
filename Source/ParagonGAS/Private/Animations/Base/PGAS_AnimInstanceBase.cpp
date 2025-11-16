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
 * Date: 7-7-2025
 * =============================================================================
 * PGAS_AnimInstanceBase.h
 * Base class for all Animation Instances in ParagonGAS.
 * This class serves as a foundation for custom animation instances, providing a common interface and functionality.
*/


#include "Animations/Base/PGAS_AnimInstanceBase.h"

// Constructor
UPGAS_AnimInstanceBase::UPGAS_AnimInstanceBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bUseMultiThreadedAnimationUpdate = true; // Enable multi-threaded animation updates
    RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly; // Set root motion mode to only from montages
}

// Called when the animation instance is destorying
void UPGAS_AnimInstanceBase::BeginDestroy()
{
    CachedMovementComponent = nullptr; // Clear cached movement component pointer
    OwningPawn = nullptr; // Clear cached owning pawn pointer
    
    Super::BeginDestroy();
}

// Initialize the animation instance
void UPGAS_AnimInstanceBase::NativeInitializeAnimation()
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
void UPGAS_AnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
}

// Called when the animation instance updates thread safe
void UPGAS_AnimInstanceBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}