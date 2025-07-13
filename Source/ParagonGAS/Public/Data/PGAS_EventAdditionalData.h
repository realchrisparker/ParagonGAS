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
 * Date: 7-12-2025
 * =============================================================================
 * PGAS_EventAdditionalData
 * Contains additional data for gameplay events (hit reactions, damage info, etc.).
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PGAS_EventAdditionalData.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (DisplayName = "Additional Event Data"))
class PARAGONGAS_API UPGAS_EventAdditionalData : public UObject
{
    GENERATED_BODY()

public:
    /*
    * Functions
    */

    // Sets default values for this component's properties
    UPGAS_EventAdditionalData();

    /*
    * Properties
    */

    // The hit result from the trace
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    FHitResult HitResult;

    // The damage event tag for the hit reaction
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    FGameplayTag DamageEventTag;
};
