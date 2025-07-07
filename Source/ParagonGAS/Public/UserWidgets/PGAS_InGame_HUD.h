// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PGAS_InGame_HUD.generated.h"

/**
 * 
 */
UCLASS()
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
