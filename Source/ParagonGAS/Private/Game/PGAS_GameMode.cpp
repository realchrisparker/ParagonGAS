// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PGAS_GameMode.h"
#include <Game/PGAS_HUD.h>
#include <Game/PGAS_GameState.h>
#include <Game/PGAS_GameSession.h>
#include <Game/PGAS_PlayerState.h>
#include <Controllers/Player/PGAS_PlayerController.h>
#include <Characters/Player/PGAS_PlayerCharacter.h>

APGAS_GameMode::APGAS_GameMode()
{
    // Set default pawn class to our custom player character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_Game/Characters/Players/Wukong/BP_Wukong.BP_Wukong_C"));
    if (PlayerPawnClassFinder.Succeeded())
    {
       DefaultPawnClass = PlayerPawnClassFinder.Class;
    }
    else
    {
        DefaultPawnClass = APGAS_PlayerCharacter::StaticClass();
    }

    // Set default player controller class
    PlayerControllerClass = APGAS_PlayerController::StaticClass();

    // Set default HUD class to our custom HUD
    HUDClass = APGAS_HUD::StaticClass();

    // Set custom HUD class
    GameStateClass = APGAS_GameState::StaticClass();

    // Set custom game state class
    PlayerStateClass = APGAS_PlayerState::StaticClass();
}

void APGAS_GameMode::BeginPlay()
{
    Super::BeginPlay();
}

TSubclassOf<AGameSession> APGAS_GameMode::GetGameSessionClass() const
{
    return APGAS_GameSession::StaticClass();
}