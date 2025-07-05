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
// #include <Interfaces/IHUD.h>
// #include <UserWidgets/PGAS_HUDUserWidgetBase.h>
#include "PGAS_HUD.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PARAGONGAS_API APGAS_HUD : public AHUD//, public IIHUD
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
	 * Get HUD Widget
	 * Returns the HUD widget in use.
	*/
	// UPGAS_HUDUserWidgetBase* GetHUDWidget() const { return HUDWidget; }

	// Override functions from IIHUD interface
	// virtual float UpdateHealthBar(float NewPercentage) const override;
    // virtual float UpdateAdrenalineBar(float NewPercentage) const override;
    // virtual float UpdateStaminaBar(float NewPercentage) const override;
	// virtual float UpdateExperienceBar(float NewPercentage) const override;

private:
	/*
	 * Properties
	*/

	// HUD Widget Class
	// UPGAS_HUDUserWidgetBase* HUDWidget = nullptr;
};
