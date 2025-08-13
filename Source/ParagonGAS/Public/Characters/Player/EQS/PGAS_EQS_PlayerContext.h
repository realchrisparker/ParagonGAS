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
 * Date: 8-12-2025
 * =============================================================================
 * PGAS_EQS_PlayerContext
 * This EQS context provides the player character and location as context for EQS queries.
 */

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_BlueprintBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"
#include <Characters/Player/PGAS_PlayerCharacter.h>
#include "PGAS_EQS_PlayerContext.generated.h"

UCLASS(Blueprintable, BlueprintType, category = "PGAS", meta = (DisplayName = "EQS Player Context"))
class PARAGONGAS_API UPGAS_EQS_PlayerContext : public UEnvQueryContext_BlueprintBase
{
    GENERATED_BODY()

public:

public:
    /** Get the world context */
    UWorld* GetWorld() const
    {
        if (const UObject* OuterObj = GetOuter())
        {
            return OuterObj->GetWorld();
        }
        return nullptr;
    }

    /**
     * Provide the context for the EQS query
     * This function sets the player character and location as context for the EQS query.
     * @param QueryInstance The EQS query instance
     * @param ContextData The context data to populate
     */
    void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
    {
        UWorld* World = GetWorld();
        if (!World)
        {
            return;
        }

        if (APGAS_PlayerCharacter* PlayerChar = Cast<APGAS_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0)))
        {
            // UEnvQueryItemType_Actor::SetContextHelper stores the actor reference in the FEnvQueryContextData, and when EQS later resolves that data,
            // it automatically uses the actor’s current transform for any location - based queries.
            UEnvQueryItemType_Actor::SetContextHelper(ContextData, PlayerChar);
        }
    }
};