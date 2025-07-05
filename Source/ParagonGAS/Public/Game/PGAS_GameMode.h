// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <Game/PGAS_GameSession.h>
#include "PGAS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class PARAGONGAS_API APGAS_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Constructor
	APGAS_GameMode();

protected:
	// Called when the game starts or when spawned
	// This is where you can initialize game state, spawn actors, etc.
	virtual void BeginPlay() override;

	// Override to specify the game session class
	// This is where you can set a custom game session class if needed
	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;
};
