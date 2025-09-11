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
#include <Structs/PGAS_HitReaction.h>
#include "PGAS_HitReactionDataObject.generated.h"


/**
 * Data object to hold a hit reaction struct
 */
UCLASS(BlueprintType, Blueprintable,
    meta = (DisplayName = "Hit Reaction Data Object",
        Description = "Represents a wrapper for hit reaction struct")
)
class PARAGONGAS_API UPGAS_HitReactionDataObject : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "Hit Reaction")
    FPGAS_HitReaction Data;
};
