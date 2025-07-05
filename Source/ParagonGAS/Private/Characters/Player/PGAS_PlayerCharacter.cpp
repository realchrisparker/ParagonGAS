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


#include "Characters/Player/PGAS_PlayerCharacter.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"

// Sets default values
APGAS_PlayerCharacter::APGAS_PlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create Camera Boom (Pulls in towards player if colliding)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetMesh()); //Attach to the Mesh
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true; // Let camera follow controller

    // Create Follow Camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false; // Don't rotate camera with pawn

    // Don't rotate camera when controller rotates
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false; // Character doesn’t rotate to match the controller’s yaw
    bUseControllerRotationRoll = false;
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // controls how fast the character turns
    GetCharacterMovement()->bUseControllerDesiredRotation = false;   // only rotate to movement, not camera

    // Set up the character's attribute set.
    // This is where you define your character's attributes like health, mana, etc.
    AttributeSet = CreateDefaultSubobject<UPlayerCharacterAttributeSet>(TEXT("AttributeSet"));

    // Default character level
    CharacterLevel = 1;

    // Add default gameplay tags.
    SetupDefaultGameplayTags();
}

// Called when the game starts or when spawned
void APGAS_PlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APGAS_PlayerController* PC = Cast<APGAS_PlayerController>(GetController()))
    {
        // Set the player state reference.
        // This is a safe cast, as the controller should always have a player state.
        MyPlayerState = Cast<APGAS_PlayerState>(PC->GetPlayerState<APlayerState>());

        // Set HUD reference.
        MyPlayerHUD = Cast<APGAS_HUD>(PC->GetHUD());
    }

    // Make sure the character always uses LOD0.
    // This is useful for player characters to ensure they always look good.
    if (GetMesh())
    {
        GetMesh()->SetForcedLOD(1);  // 0 = Auto, 1 = LOD0, 2 = LOD1, etc.
    }

    // SetupDefaultAbilities();
}

// Called every frame
void APGAS_PlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called when the player has been possessed by a controller
void APGAS_PlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);


    // Ensure the character has a valid PlayerState
    try {
        MyPlayerController = Cast<APGAS_PlayerController>(NewController);
        if (!MyPlayerController)
        {
            UE_LOG(LogTemp, Warning, TEXT("APGAS_PlayerCharacter::PossessedBy - MyPlayerController is null!"));
        }
    }
    catch (const std::exception& e)
    {
        UE_LOG(LogTemp, Error, TEXT("APGAS_PlayerCharacter::PossessedBy - Exception: %s"), *FString(e.what()));
    }

    ApplyDefaultAttributeEffects();
}

// Called when player state has been replicated
void APGAS_PlayerCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
}

// Called to bind functionality to input
void APGAS_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced Input requires an EnhancedInputComponent to bind actions.
    if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Ensure we actually have the IA_Move asset set
        if (IA_Move)
        {
            EnhancedInputComp->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APGAS_PlayerCharacter::MoveAction);
            EnhancedInputComp->BindAction(IA_Move, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::MoveStartedAction);
            EnhancedInputComp->BindAction(IA_Move, ETriggerEvent::Completed, this, &APGAS_PlayerCharacter::MoveStopAction);
            EnhancedInputComp->BindAction(IA_Move, ETriggerEvent::Canceled, this, &APGAS_PlayerCharacter::MoveStopAction);
        }
        if (IA_Look)
        {
            EnhancedInputComp->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APGAS_PlayerCharacter::LookAction);
        }
        if (IA_Jump)
        {
            EnhancedInputComp->BindAction(IA_Jump, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::JumpAction);
        }
        if (IA_PrimaryAttack)
        {
            EnhancedInputComp->BindAction(IA_PrimaryAttack, ETriggerEvent::Triggered, this, &APGAS_PlayerCharacter::PrimaryAttackAction);
        }
    }
}

/*
 * MoveStartedAction function to handle the start of movement input.
 * This function is called when the IA_Move input action is started.
*/
void APGAS_PlayerCharacter::MoveStartedAction(const FInputActionInstance& Value)
{
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Jogging"))); // This tag indicates the character is jogging or moving.
}

/*
 * Move function to handle movement input.
 * This function is called when the IA_Move input action is triggered.
*/
void APGAS_PlayerCharacter::MoveAction(const FInputActionInstance& Value)
{
    // The input value is expected to be a Vector2D: X (strafe), Y (forward).
    FInputActionValue ActionValue = Value.GetValue();
    const FVector2D MovementVector = ActionValue.Get<FVector2D>();

    if (!Controller || MovementVector.IsNearlyZero())
    {
        return;
    }

    // Forward/backward movement
    if (FMath::Abs(MovementVector.Y) > 0.0f)
    {
        const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
        const FVector DirectionForward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(DirectionForward, MovementVector.Y);
    }

    // Right/left movement
    if (FMath::Abs(MovementVector.X) > 0.0f)
    {
        const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
        const FVector DirectionRight = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(DirectionRight, MovementVector.X);

    }
}

/*
 * This function is called when the IA_Move input action is completed or canceled.
*/
void APGAS_PlayerCharacter::MoveStopAction(const FInputActionInstance& Value)
{
    // Remove character's movement gameplay tags. (Any gameplay effects tied to movement tags will stop.)
    RemoveGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Jogging")));
}

/*
 * Look function to handle looking around input.
 * This function is called when the IA_Look input action is triggered.
*/
void APGAS_PlayerCharacter::LookAction(const FInputActionValue& Value)
{
    // IA_Look is set up to provide a 2D axis (X = turn, Y = look up/down).
    const FVector2D LookAxisValue = Value.Get<FVector2D>();

    if (Controller)
    {
        // Add yaw input (horizontal turn).
        AddControllerYawInput(LookAxisValue.X);

        // Add pitch input (vertical look).
        AddControllerPitchInput(LookAxisValue.Y);
    }
}

/*
 * Jump function to handle jumping input.
 * This function is called when the IA_Jump input action is triggered.
*/
void APGAS_PlayerCharacter::JumpAction(const FInputActionValue& Value)
{
    // Early-out if we're already falling (i.e., in the air).
    if (GetCharacterMovement() && GetCharacterMovement()->IsFalling())
    {
        return;
    }

    // Otherwise, we're on the ground, so jump now.
    Jump();
}

/*
 * PrimaryAttackAction function to handle primary attack input.
 * This function is called when the IA_PrimaryAttack input action is triggered.
*/
void APGAS_PlayerCharacter::PrimaryAttackAction(const FInputActionValue& Value)
{
    // if (IsAttacking() && bCanComboAttack)
    // {
    // 	UPGAS_AbilitySystemComponent* ASC = Cast<UPGAS_AbilitySystemComponent>(GetAbilitySystemComponent());
    // 	if (ASC)
    // 	{
    // 		UPGAS_GameplayAbility_Montage* LastAbility = Cast<UPGAS_GameplayAbility_Montage>(ASC->LastActivatedAbility);
    // 		if (LastAbility)
    // 		{
    // 			// If the character is already attacking, we send input request to the ability to let it know it might need to activate a combo.
    // 			LastAbility->TryActivateNextCombo();
    // 		}
    // 	}

    // 	return;
    // }

    // Step 1: Listen for melee ability state notify events.
    // Step 2: Player presses the primary attack button after the ability has been activated we need to check if the combo window is still open.
    // Step 3: If the combo window is still open, we activate the next combo ability.

    // Activate the melee GAS ability.
    // if (ActivateMeleeAbility(true))
    // {
    //     SetIsAttacking(true); // Set the attacking flag to true
    // }
}

/*
 * SetupDefaultGameplayTags function to set up default gameplay tags for this character.
 * This is typically called in the constructor or BeginPlay.
*/
void APGAS_PlayerCharacter::SetupDefaultGameplayTags()
{
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Type.Player")));
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Alive")));
}

// void APGAS_PlayerCharacter::SetupDefaultAttributes()
// {
//     // Create and apply the default attribute gameplay effect
//     UPGAS_GE_PlayerDefaultAttributes* DefaultAttributesEffect = NewObject<UPGAS_GE_PlayerDefaultAttributes>();
//     if (DefaultAttributesEffect)
//     {
//         // Apply the gameplay effect to the character
//         GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(DefaultAttributesEffect, 1.0f, GetAbilitySystemComponent()->MakeEffectContext());
//     }
// }

// Handles changes to the character's health attribute
void APGAS_PlayerCharacter::HandleHealthChange(float DeltaValue, AActor* Causer)
{
    OnHealthChanged(DeltaValue, Causer);
    if (GetHealth() <= 0)
    {
        // If the character's health is zero or less, trigger the death event
        OnDeath();
    }
}

// Handles changes to the character's mana attribute
void APGAS_PlayerCharacter::HandleAdrenalineChange(float DeltaValue, AActor* Causer)
{
    OnAdrenalineChanged(DeltaValue, Causer);
    // if(GetAdrenaline() <= 0)
    // {
    // 	// If the character's adrenaline is zero or less, trigger the death event
    // 	OnAdrenalineDepleted();
    // }
}

// Handles changes to the character's stamina attribute
void APGAS_PlayerCharacter::HandleStaminaChange(float DeltaValue, AActor* Causer)
{
    OnStaminaChanged(DeltaValue, Causer);
    // if(GetStamina() <= 0)
    // {
    // 	// If the character's stamina is zero or less, trigger the death event
    // 	OnStaminaDepleted();
    // }
}

// Handles changes to the character's experience points
void APGAS_PlayerCharacter::HandleExperiencePointsChange(float DeltaValue)
{
    OnExperiencePointsChanged(DeltaValue);
}

// Handles the character's level-up logic
void APGAS_PlayerCharacter::HandleCharacterLevelUp()
{
    CharacterLevel++; // Increment the character's level
    RemoveDefaultAttributeEffects();
    ApplyDefaultAttributeEffects(); // Reapply the default attribute effects for the new level
    OnCharacterLevelUp(); // Trigger the level-up event
}
