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
 * Date: 10-12-2025
 * =============================================================================
 * Description:
 * Camera shake that plays when the player lands a melee hit or receives one.
 * Uses the new UCameraShakeBase system with configurable Perlin Noise pattern.
 * =============================================================================
 */

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "Shakes/PerlinNoiseCameraShakePattern.h"
#include "PGAS_CameraShake_HitImpact.generated.h"

/**
 * A simple camera shake for melee hit impacts.
 */
UCLASS(Blueprintable, BlueprintType, meta = (DisplayName = "PGAS Camera Shake Hit Impact"))
class PARAGONGAS_API UPGAS_CameraShake_HitImpact : public UCameraShakeBase
{
    GENERATED_BODY()

public:
    UPGAS_CameraShake_HitImpact(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
        : Super(ObjectInitializer)
    {
        // ✅ Proper way to create a subobject inside a UObject constructor
        UPerlinNoiseCameraShakePattern* Pattern =
            ObjectInitializer.CreateDefaultSubobject<UPerlinNoiseCameraShakePattern>(this, TEXT("PerlinPattern"));

        // --- Translation shake (position) ---
        Pattern->X.Amplitude = 2.5f;
        Pattern->X.Frequency = 20.0f;
        Pattern->Y.Amplitude = 2.5f;
        Pattern->Y.Frequency = 20.0f;
        Pattern->Z.Amplitude = 1.0f;
        Pattern->Z.Frequency = 15.0f;

        // --- Rotational shake (pitch/yaw) ---
        Pattern->Pitch.Amplitude = 1.5f;
        Pattern->Pitch.Frequency = 25.0f;
        Pattern->Yaw.Amplitude = 1.5f;
        Pattern->Yaw.Frequency = 25.0f;

        // --- Duration ---
        Pattern->Duration = 0.3f;

        // Assign as the root shake pattern
        SetRootShakePattern(Pattern);
    }
};
