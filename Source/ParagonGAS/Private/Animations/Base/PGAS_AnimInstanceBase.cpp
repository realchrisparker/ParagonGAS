// Fill out your copyright notice in the Description page of Project Settings.


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
    Super::BeginDestroy();

    try
    {
        CachedMovementComponent = nullptr; // Clear cached movement component pointer
        OwningPawn = nullptr; // Clear cached owning pawn pointer
    }
    catch (...)
    {
    }
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