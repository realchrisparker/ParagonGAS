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
 * Date: 7-4-2025
 * =============================================================================
 * PGAS_GE_JoggingStaminaReduction
 * Gameplay Effect for reducing stamina while jogging (C++ version).
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "PGAS_GE_JoggingStaminaReduction.generated.h"

UCLASS(BlueprintType, Blueprintable, Category = "Gameplay Ability System|Effects", meta = (DisplayName = "Jogging Stamina Reduction"))
class PARAGONGAS_API UPGAS_GE_JoggingStaminaReduction : public UGameplayEffect
{
    GENERATED_BODY()

public:
    UPGAS_GE_JoggingStaminaReduction();
};