/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * Author: Christopher D. Parker
 * Date: 9-6-2025
 * =============================================================================
 * Defines hit reaction enums and struct for data-driven hit reactions.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimMontage.h"
#include <Enums/PGAS_HitDirection.h>
#include "PGAS_HitReaction.generated.h"

 /**
   * Data-driven hit reaction struct.
   * Used instead of hard-coded montages for flexibility.
   */
USTRUCT(BlueprintType, Blueprintable, meta = (DisplayName = "Hit Reaction"))
struct PARAGONGAS_API FPGAS_HitReaction
{
  GENERATED_BODY()

public:

  /** The ability tag associated with this reaction (e.g., "Character.Ability.HitReaction"). */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction")
  FGameplayTag AbilityTag;

  /** The direction this reaction is valid for. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction")
  EPGAS_HitDirection Direction = EPGAS_HitDirection::None;

  /** The severity of the hit (light, heavy, stagger, knockback, knockdown, dazed, critical). */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction")
  EPGAS_HitSeverity Severity = EPGAS_HitSeverity::Light;

  /** The animation montage to play for this reaction. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction")
  TObjectPtr<UAnimMontage> Montage;

  /** Optional play rate for the montage. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction", meta = (ClampMin = "0.1", UIMin = "0.1"))
  float PlayRate = 1.0f;

  // ------------------------
  // Knockdown-specific
  // ------------------------

  /** Time to pause after knockdown while character is on the ground before get-up. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction|Knockdown", meta = (DisplayName = "On Ground Time", ClampMin = "0.0", UIMin = "0.0"))
  float OnGround = 1.0f;

  /** Montage to play when character gets up from knockdown. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction|Knockdown", meta = (DisplayName = "Get Up Montage"))
  TObjectPtr<UAnimMontage> GetUpMontage;

  // ------------------------
  // Dazed-specific
  // ------------------------

  /** Time character remains dazed. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction|Dazed", meta = (DisplayName = "Dazed Time"))
  float DazedTime = 1.0f;

  /** Montage to play when recovering from dazed state. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction|Dazed", meta = (DisplayName = "Recovery Montage"))
  TObjectPtr<UAnimMontage> RecoveryMontage;
};
