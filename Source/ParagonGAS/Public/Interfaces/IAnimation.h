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
 #include "IAnimation.generated.h"

 UINTERFACE(BlueprintType, meta = (DisplayName = "PGAS Animation Interface"))
     class PARAGONGAS_API UIAnimation : public UInterface
 {
     GENERATED_BODY()
 };

 class PARAGONGAS_API IIAnimation
 {
     GENERATED_BODY()
 
 public:
     
    // Starts blocking with the character.
     UFUNCTION(Category = "Animation",
    meta = (AllowPrivateAccess = "true",
        DisplayName = "Start Blocking",
        Keywords = "combat block",
        Tooltip = "Starts blocking with the character.")
    )
    virtual void StartBlocking() = 0;

    // Stops blocking with the character.
    UFUNCTION(Category = "Animation",
        meta = (AllowPrivateAccess = "true",
            DisplayName = "Stop Blocking",
            Keywords = "combat block",
            Tooltip = "Stops blocking with the character.")
    )
    virtual void StopBlocking() = 0;
 };
  