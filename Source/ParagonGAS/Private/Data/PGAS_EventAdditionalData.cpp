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


#include "Data/PGAS_EventAdditionalData.h"

UPGAS_EventAdditionalData::UPGAS_EventAdditionalData()
{
    // Initialize default values
    HitResult = FHitResult();
    DamageEventTag = FGameplayTag::EmptyTag;
}