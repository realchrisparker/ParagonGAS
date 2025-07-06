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
 * Date: 7-1-2025
 * =============================================================================
 * ICharacterCombat Interface
 * Blueprint-pure interface for character combat actions.
 */

 #pragma once

 #include "CoreMinimal.h"
 #include "UObject/Interface.h"
 #include "ICharacterCombat.generated.h"

 UINTERFACE(BlueprintType, meta = (DisplayName = "Character Combat Interface"))
 class PARAGONGAS_API UICharacterCombat : public UInterface
 {
     GENERATED_BODY()
 };

 class PARAGONGAS_API IICharacterCombat
 {
     GENERATED_BODY()
 
 public:
     
    /**
      * Performs a weapon trace to detect hits.
      * This function should be implemented to perform a trace from Start to End.
      * @return void.
    */
     UFUNCTION(Category = "Character|Combat", 
        meta = (AllowPrivateAccess = "true", 
            DisplayName = "Weapon Trace",
            Keywords = "combat trace weapon hit detection", 
            Tooltip = "Performs a weapon trace to detect hits."))
     virtual void WeaponTrace() = 0;
 };
  