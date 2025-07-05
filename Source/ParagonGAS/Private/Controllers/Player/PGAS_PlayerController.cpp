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


#include "Controllers/Player/PGAS_PlayerController.h"


APGAS_PlayerController::APGAS_PlayerController()
{
    // Load your Input Mapping Context asset from disk
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_DefaultRef(TEXT("/Game/_Game/Input/IMC_Default.IMC_Default"));
    if (IMC_DefaultRef.Succeeded())
    {
        DefaultMappingContext = IMC_DefaultRef.Object;
    }

    bEnableStreamingSource = true; // Enable streaming source by default
}

/**
 * Called when the game starts or when spawned.
 * This is where we set up the input mapping contexts for the player controller.
 */
void APGAS_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Make sure this is the local controller (not a remote client or dedicated server).
    if (IsLocalController())
    {
        // Retrieve the LocalPlayer associated with this PlayerController
        if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
        {
            // Get the Enhanced Input Local Player Subsystem
            if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                // (Optional) Clear existing contexts
                InputSubsystem->ClearAllMappings();

                // Add our default mapping context
                if (DefaultMappingContext)
                {
                    InputSubsystem->AddMappingContext(DefaultMappingContext, /*Priority=*/0);
                }

                // Optionally, you can also add other mapping contexts here
                // InputSubsystem->AddMappingContext(OtherMappingContext, /*Priority=*/1);
            }
        }
    }
}

void APGAS_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
}
