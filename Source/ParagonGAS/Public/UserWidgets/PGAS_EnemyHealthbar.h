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
* PGAS_EnemyHealthbar.h
* This file defines the enemy health bar widget in the game.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PGAS_EnemyHealthbar.generated.h"

/**
 * Enemy Healthbar Widget
*/
UCLASS(meta= (DisplayName = "Enemy Healthbar Widget"))
class PARAGONGAS_API UPGAS_EnemyHealthbar : public UUserWidget
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
	 * Updates the health bar's visibility based on whether the enemy is alive or dead.
	 * @param bIsAlive Whether the enemy is alive or not. If true, the health bar will be visible; if false, it will be hidden.
	*/
	void UpdateHealthBarVisibility(bool bIsAlive);

	/**
	 * Properties
	*/

	// Progress bars for health
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> Healthbar;
};
