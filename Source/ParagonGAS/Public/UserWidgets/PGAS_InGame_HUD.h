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
* Date: 7-6-2025
* =============================================================================
* PGAS_InGame_HUD.h
* This file defines the in-game HUD widget.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PGAS_InGame_HUD.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent, DisplayName = "In-Game HUD"))
class PARAGONGAS_API UPGAS_InGame_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	/*
	 * Functions
	*/

	/*
	 * Override BeginDestroy to clean up references
	 * This is where you can clean up any references or perform final actions before the widget is removed
	*/
	virtual void BeginDestroy() override;

	/**
	 * Update the health value displayed on the HUD
	 * @param NewValue The new health value to display
	 * @param MaxValue The maximum health value to normalize against
	*/
	void UpdateHealthValue(float NewValue, float MaxValue) const;

	/**
	 * Update the stamina value displayed on the HUD
	 * @param NewValue The new stamina value to display
	 * @param MaxValue The maximum health value to normalize against
	*/
	void UpdateStaminaValue(float NewValue, float MaxValue) const;

	/**
	 * Update the adrenaline value displayed on the HUD
	 * @param NewValue The new adrenaline value to display
	 * @param MaxValue The maximum health value to normalize against
	*/
	void UpdateAdrenalineValue(float NewValue, float MaxValue) const;

	/**
	 * Properties
	*/

	// Progress bars for health
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> Healthbar;

	// Progress bar for stamina
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> Staminabar;

	// Progress bar for adrenaline
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> Adrenalinebar;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite,meta = (BindWidget))
	// TObjectPtr<UProgressBar> Experiencebar;
};
