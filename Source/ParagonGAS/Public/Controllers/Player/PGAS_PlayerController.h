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
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedPlayerInput.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerInput.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "PGAS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PARAGONGAS_API APGAS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Constructor
	APGAS_PlayerController();


protected:
	/*
	* Properties
	*/

	/** Reference to our default input mapping context (IMC_Default). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/*
	* Functions
	*/

	/**
	 * Called when the game starts or when spawned.
	 * This is where we set up the input mapping contexts for the player controller.
	*/
	virtual void BeginPlay() override;

	/**
	 * Sets up the input component for the player controller.
	 * This function is called to initialize the input component and bind input actions.
	*/
	virtual void SetupInputComponent() override;
};
