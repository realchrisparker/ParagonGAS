// Fill out your copyright notice in the Description page of Project Settings.


#include <Game/PGAS_GameMode.h>
#include <Game/PGAS_HUD.h>
#include <Game/PGAS_GameState.h>
#include <Game/PGAS_GameSession.h>
#include <Game/PGAS_PlayerState.h>
#include <Controllers/Player/PGAS_PlayerController.h>
#include <Characters/Player/PGAS_PlayerCharacter.h>
#include <Game/PGAS_GameInstance.h>


APGAS_GameMode::APGAS_GameMode()
{
    // Set default pawn class to our custom player character.
    DefaultPawnClass = APGAS_PlayerCharacter::StaticClass();

    // Set default player controller class
    PlayerControllerClass = APGAS_PlayerController::StaticClass();

    // Set default HUD class to our custom HUD
    HUDClass = APGAS_HUD::StaticClass();

    // Set custom HUD class
    GameStateClass = APGAS_GameState::StaticClass();

    // Set custom game state class
    PlayerStateClass = APGAS_PlayerState::StaticClass();
}

void APGAS_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    // Access the custom GameInstance to get the selected pawn class
    if (UGameInstance* GI = GetGameInstance())
    {
        // Cast to our custom GameInstance class
        if (UPGAS_GameInstance* PGI = Cast<UPGAS_GameInstance>(GI))
        {
            if (PGI->GetSelectedPawnClass())
            {
                DefaultPawnClass = PGI->GetSelectedPawnClass(); // Set the DefaultPawnClass from GameInstance
            }
        }
    }
}

void APGAS_GameMode::BeginPlay()
{
    Super::BeginPlay();
}

TSubclassOf<AGameSession> APGAS_GameMode::GetGameSessionClass() const
{
    return APGAS_GameSession::StaticClass();
}

void APGAS_GameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
}

/*
 * Override SpawnDefaultPawnFor to customize pawn spawning logic
 * This is where you can modify how pawns are spawned for players
 * @param NewPlayer The player controller for whom the pawn is being spawned
 * @param StartSpot The actor representing the spawn location
 */
APawn* APGAS_GameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
    UE_LOG(LogTemp, Warning, TEXT("Spawning pawn: %s"), *GetNameSafe(DefaultPawnClass));
    return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}
