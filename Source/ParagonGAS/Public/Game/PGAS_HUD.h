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
	* Date: 6-28-2025
	* =============================================================================
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include <Interfaces/IHUD.h>
#include <UserWidgets/PGAS_InGame_HUD.h>
#include "PGAS_HUD.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PARAGONGAS_API APGAS_HUD : public AHUD, public IIHUD
{
	GENERATED_BODY()

public:
	/*
	* Functions
	*/
	// Constructor
	APGAS_HUD();

	// Override BeginPlay to initialize HUD
	virtual void BeginPlay() override;

	/*
	 * Get In-Game HUD Widget
	 * Returns the In-Game HUD widget in use.
	*/
	UPGAS_InGame_HUD* GetInGameHUDWidget() const { return InGameHUDWidget; }

	// Override functions from IIHUD interface
	virtual float UpdateHealthBar(float NewValue, float MaxValue) const override;
    virtual float UpdateAdrenalineBar(float NewValue, float MaxValue) const override;
    virtual float UpdateStaminaBar(float NewValue, float MaxValue) const override;
	virtual float UpdateExperienceBar(float NewValue, float MaxValue) const override;

private:
	/*
	 * Properties
	*/

	// In-Game HUD Widget Class
	UPGAS_InGame_HUD* InGameHUDWidget = nullptr;
};
