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
 * PGAS_PlayerAnimInstanceBase.cpp
 * Base class for all Player Animation Instances in ParagonGAS.
*/

#include "Animations/Base/PGAS_PlayerAnimInstanceBase.h"
#include "Kismet/KismetMathLibrary.h"

// Constructor
UPGAS_PlayerAnimInstanceBase::UPGAS_PlayerAnimInstanceBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    //
}

// Called when the animation instance is destorying
void UPGAS_PlayerAnimInstanceBase::BeginDestroy()
{
    Super::BeginDestroy();

    try
    {
        OwningCharacter = nullptr; // Clear the cached character pointer
    }
    catch (...)
    {
    }
}

// Initialize the animation instance
void UPGAS_PlayerAnimInstanceBase::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Initialize any variables or perform setup here (OwningPawn found in base class)
    APawn* OPawn = TryGetPawnOwner();
    if (OPawn)
    {
        OwningCharacter = Cast<APGAS_PlayerCharacter>(OPawn); // Cache the owning character
    }
}

// Called when the animation instance updates
void UPGAS_PlayerAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
}

// Called when the animation instance updates thread safe
void UPGAS_PlayerAnimInstanceBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

    // Perform any thread-safe updates here
    if (GetMovementComponent() && GetOwningPawn())
    {
        /**
         * Update IsInAir based on the movement component
        */
        IsInAir = GetMovementComponent()->IsFalling();

        /**
         * Update velocity.
         */
        Velocity = GetMovementComponent()->Velocity;

        /**
         * Update speed.
        */
        Speed = GetOwningPawn()->GetVelocity().Length();

        /**
         * Update Roll, Pitch, and Yaw based on the owning pawn's rotation.
         */

        // Get owning pawn's rotation & base aim rotation
        FRotator AimRotation = GetOwningPawn()->GetBaseAimRotation(); // This is the rotation the pawn is aiming at
        FRotator ActorRotation = GetOwningPawn()->GetActorRotation(); // This is the rotation of the pawn itself
        FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation); // Calculate the delta rotation between aim and actor rotation

        // Update Roll, Pitch, and Yaw based on the delta rotation
        Roll = DeltaRot.Roll;
        Pitch = DeltaRot.Pitch;
        Yaw = DeltaRot.Yaw;

        /**
         * Update Direction based on velocity and actor rotation.
         */
        Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, ActorRotation);
        SetEDirection();

        /**
         * Update Yaw Delta for leans
        */

        // Calculate the delta rotation between Rotation Last Tick and actor rotation
        FRotator DeltaRot2 = UKismetMathLibrary::NormalizedDeltaRotator(RotationLastTick, ActorRotation);

        const float leanIntensity = 7.0f; // Adjust this value to control the intensity of the lean
        const float InterpSpeed = 6.0f;
        const float DeltaTime = 0.0f;
        float CurrentYawDelta = DeltaRot2.Yaw; // your existing float value
        float TargetYawDelta = (DeltaRot.Yaw / 1.f) / leanIntensity;        
        YawDelta = FMath::FInterpTo(CurrentYawDelta, TargetYawDelta, DeltaTime, InterpSpeed);

        /**
         * Update rotation last tick.
        */        
        RotationLastTick = ActorRotation;

        /**
         * Update IsAccelerating based on the velocity.
        */
        IsAccelerating = GetMovementComponent()->GetCurrentAcceleration().Size() > 2.0f;

        // Update full body flag based on the "FullBody" curve value
        FullBody = GetCurveValue(FName("FullBody")) > 0.0f;

        // Update IsCrouched based on the owning pawn's crouch state
        IsCrouched = GetOwningCharacter()->bIsCrouched;

        // ============================
        // Determine CharacterAnimState
        // ============================
        if (IsInAir) // In Air
        {
            CharacterAnimState = EPGAS_CharacterAnimState::InAir;
        }
        else if (IsBlocking)
        {
            if (Speed < 10.f)
            {
                CharacterAnimState = EPGAS_CharacterAnimState::BlockingIdle;
            }
            else if (Speed < WalkMaxSpeed)
            {
                CharacterAnimState = EPGAS_CharacterAnimState::BlockingWalking;
            }
            else if (Speed < JogMaxSpeed)
            {
                CharacterAnimState = EPGAS_CharacterAnimState::BlockingJogging;
            }
            else
            {
                CharacterAnimState = EPGAS_CharacterAnimState::BlockingRunning;
            }
        }
        else if (IsCrouched)
        {
            if (Speed < 10.f)
            {
                CharacterAnimState = EPGAS_CharacterAnimState::CrouchedIdle;
            }
            else
            {
                CharacterAnimState = EPGAS_CharacterAnimState::CrouchedWalking;
            }
        }
        else // Standing & not blocking
        {
            if (Speed < 10.f)
            {
                CharacterAnimState = EPGAS_CharacterAnimState::StandingIdle;
            }
            else if (Speed < WalkMaxSpeed)
            {
                CharacterAnimState = EPGAS_CharacterAnimState::Walking;
            }
            else if (Speed < JogMaxSpeed)
            {
                CharacterAnimState = EPGAS_CharacterAnimState::Jogging;
            }
            else
            {
                CharacterAnimState = EPGAS_CharacterAnimState::Running;
            }
        }
    }
}