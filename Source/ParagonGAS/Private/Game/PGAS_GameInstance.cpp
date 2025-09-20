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


#include "Game/PGAS_GameInstance.h"


void UPGAS_GameInstance::Init()
{
    Super::Init();

    // Set default pawn class to our custom player character. (In the future, this will be set based on player selection.)
    //static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_Game/Characters/Players/Wukong/BP_Wukong.BP_Wukong_C"));
    // static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_Game/Characters/Players/Countess/BP_Countess.BP_Countess_C"));
    // static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_Game/Characters/Players/Sparrow/BP_Sparrow.BP_Sparrow_C"));
    // Load a Blueprint class dynamically at runtime
    TSubclassOf<APawn> PawnBPClass = LoadClass<APawn>(
        nullptr,
        TEXT("/Game/_Game/Characters/Players/Sparrow/BP_Sparrow.BP_Sparrow_C")
    );

    if (PawnBPClass)
    {
        SelectedPawnClass = PawnBPClass;
        UE_LOG(LogTemp, Warning, TEXT("GameInstance Init: SelectedPawnClass set to %s"), *SelectedPawnClass->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance Init: Could not load Sparrow BP"));
    }
}
