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
 * Date: 9-20-2025
 * =============================================================================
 * PGAS_GameInstance.h
 * Extends UGameInstance to store globally accessible game data, such as which
 * character the player selected before the game starts.
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PGAS_GameInstance.generated.h"

/**
 * Custom GameInstance for global game data.
 */
UCLASS()
class PARAGONGAS_API UPGAS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/*
	 * Functions
	*/

	// Override Init to set up defaults
	virtual void Init() override;
	
	/** Sets the pawn class manually in code or Blueprint. */
	UFUNCTION(BlueprintCallable, Category = "Player|Character Selection")
	void SetSelectedPawnClass(TSubclassOf<APawn> InPawnClass) { SelectedPawnClass = InPawnClass; }

	/** Gets the currently selected pawn class. */
	UFUNCTION(BlueprintCallable, Category = "Player|Character Selection")
	TSubclassOf<APawn> GetSelectedPawnClass() const { return SelectedPawnClass; }

	/*
	 * Properties
	*/

	/** The pawn class chosen for the player (set manually or by a selection menu in future). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Character Selection")
	TSubclassOf<APawn> SelectedPawnClass;
};
