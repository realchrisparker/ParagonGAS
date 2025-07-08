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
#include <GAS/Abilities/PGAS_GameplayAbility_Montage.h>

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

    /**
     * Set up the character's attribute set.
     * This is where you define your character's attributes like health, mana, etc.
     * Unreal Engine will automatically replicate this attribute set to the client.
     * Unreal Engine GAS will automatically load this attribute set when the Ability System Component is created.
     */
    AttributeSet = CreateDefaultSubobject<UPlayerCharacterAttributeSet>(TEXT("AttributeSet"));

    // Default character level
    SetCharacterLevel(1);

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

    UpdateInGameHUD(); // Update the in-game HUD with the current player status
    // Call UpdateInGameHUD every 1/3 second
    GetWorldTimerManager().SetTimer(
        HUDUpdateTimerHandle,
        this,
        &APGAS_PlayerCharacter::UpdateInGameHUD,
        0.33f, // interval in seconds
        true  // looping
    );
}

void APGAS_PlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
}

// Called every frame
void APGAS_PlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Only track idle if we're not already playing idle anim
    if (bIdleAnimationPlayed == false)
    {
        IdleTime += DeltaTime;
        if (IdleTime >= 30.f)
        {
            PlayIdleBreakMontage();
            bIdleAnimationPlayed = false;
            IdleTime = 0.f; // Reset idle time after playing idle animation
        }
    }
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

    ApplyDefaultAttributeEffects(); // Apply default attribute effects to the character
    SetupDefaultAbilities(); // Set up default abilities for the player character
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
    if (HasGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove"))) == false)
    {
        return;
    }

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

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
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

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * Jump function to handle jumping input.
 * This function is called when the IA_Jump input action is triggered.
*/
void APGAS_PlayerCharacter::JumpAction(const FInputActionValue& Value)
{
    if (HasGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove"))) == false)
    {
        return;
    }

    // Early-out if we're already falling (i.e., in the air).
    if (GetCharacterMovement() && GetCharacterMovement()->IsFalling())
    {
        return;
    }

    // Otherwise, we're on the ground, so jump now.
    Jump();

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
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
    if (ActivateMeleeAbility(true))
    {
        SetIsAttacking(true); // Set the attacking flag to true
    }

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * SetupDefaultGameplayTags function to set up default gameplay tags for this character.
 * This is typically called in the constructor or BeginPlay.
*/
void APGAS_PlayerCharacter::SetupDefaultGameplayTags()
{
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Type.Player")));
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Alive")));
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove")));
}

/*
 * Gives the player character default abilities.
 * This function overrides the base class implementation to provide custom functionality. 
 * @note It calls the base class's SetupDefaultAbilities function to ensure any inherited functionality is executed.
*/
void APGAS_PlayerCharacter::SetupDefaultAbilities()
{
    // Call the base class implementation to ensure any inherited functionality is executed.
    Super::SetupDefaultAbilities();

    // Check if the Ability System Component is valid.
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    if (ASC)
    {
        // Give the player character a melee ability.
        if (MeleeAbility)
            MeleeAbilitySpecHandle = ASC->GiveAbility(FGameplayAbilitySpec(MeleeAbility, GetCharacterLevel(), INDEX_NONE, this));
    }
}

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
    SetCharacterLevel(GetCharacterLevel() + 1); // Increment the character's level
    RemoveDefaultAttributeEffects();
    ApplyDefaultAttributeEffects(); // Reapply the default attribute effects for the new level
    OnCharacterLevelUp(); // Trigger the level-up event
}

/*
 * Performs a weapon trace to detect hits.
 * This function should be implemented to perform a trace from Start to End.
 * @return void.
*/
void APGAS_PlayerCharacter::WeaponTrace()
{
    UWorld* World = GetWorld();
    if (!World) return;

    // Get the start and end locations of the staff sockets.
    FVector Start = GetStaffStartSocketLocation();
    FVector End = GetStaffEndSocketLocation();
    float SphereRadius = 25.0f;

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(this); // Ignore self in the trace

    TArray<FHitResult> OutHits;

    // Perform a sphere trace to detect hits.
    // This will check for any actors of the specified object types within the sphere radius between Start and End.
    bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
        World,
        Start,
        End,
        SphereRadius,
        ObjectTypes,
        false,
        IgnoreActors, // Actors to ignore
        EDrawDebugTrace::ForDuration,
        OutHits,
        true
    );

    if (bHit)
    {
        for (const FHitResult& Hit : OutHits)
        {
            // Get the actor that was hit.
            AActor* HitActor = Hit.GetActor();
            if (!HitActor) continue;

            // Check if the hit actor is valid and not already hit in this window.
            if (AlreadyHitActors.Contains(HitActor)) continue;

            // Actor not hit yet, so we add it to the set of already hit actors.
            AlreadyHitActors.Add(HitActor);

            // Prepare the event tag (must match what your abilities expect)
            FGameplayTag DamageEventTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Melee.Staff"));

            // Create Instigator Tags
            FGameplayTagContainer InstigatorTags = this->GetGameplayTags(); // Get the character's gameplay tags at this moment

            // Create optional object to hold hit information
            // UPGAS_EventDataOptionalObj* EvnDataOptObj = NewObject<UPGAS_EventDataOptionalObj>();
            // EvnDataOptObj->HitInfo = Hit;

            // Prepare the event data
            FGameplayEventData EventData;
            EventData.EventTag = DamageEventTag; // The event tag for the damage event
            // EventData.OptionalObject = EvnDataOptObj; // Optional object to hold additional data
            EventData.Instigator = this; // The actor that initiated the event (this character)
            EventData.InstigatorTags = InstigatorTags; // Tags from the instigator
            EventData.Target = HitActor; // The actor that was hit
            // EventData.TargetTags = TargetTags; // Tags for the target
            // EventData.ContextHandle = /* optional FGameplayEffectContextHandle, if you have one */;
            // EventData.EventMagnitude = 25.0f;
            // EventData.OptionalObject2 = nullptr; // Optional second object, if needed

            // Send the event
            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, DamageEventTag, EventData);
        }
    }
}

/**
 * Activates the character's melee ability
 * This function checks if the character can activate the melee ability and performs the activation.
 * @param AllowRemoteActivation Whether to allow remote activation of the ability (default: true
*/
bool APGAS_PlayerCharacter::ActivateMeleeAbility(bool AllowRemoteActivation)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

    // Ensure the AbilitySystemComponent and MeleeAbilitySpecHandle are valid before proceeding
    if (!ASC || !MeleeAbilitySpecHandle.IsValid())
        return false;

    // Bind to our events to handle montage state notifications in C++
    UPGAS_GameplayAbility_Montage::OnMontageStateNotify.RemoveAll(this); // Unbind any previous bindings to avoid duplicates
    UPGAS_GameplayAbility_Montage::OnMontageStateNotify.AddUObject(this, &APGAS_PlayerCharacter::HandleMontageStateNotify); // Bind

    // Activate the ability
    return ASC->TryActivateAbility(MeleeAbilitySpecHandle, AllowRemoteActivation);
}

/**
 * Handles the montage state notify events.
 * This function is called when a montage state notify event occurs.
 * @param NotifyTag The tag associated with the notify event.
 * @param EventData The event data associated with the notify event.
*/
void APGAS_PlayerCharacter::HandleMontageStateNotify(FGameplayTag NotifyTag, FGameplayEventData EventData)
{
    UE_LOG(LogTemp, Log, TEXT("HandleMontageStateNotify called with NotifyTag: %s"), *NotifyTag.ToString());
    // Determine the type of notify and handle it accordingly
    if (NotifyTag == FGameplayTag::RequestGameplayTag(FName("Attack.Damage.Notify.Begin")))
    {
        AlreadyHitActors.Empty(); // Clear at the start of the window!

        // Start tracing repeatedly using a timer
        GetWorldTimerManager().SetTimer(
            WeaponTraceTimerHandle,
            this,
            &APGAS_PlayerCharacter::WeaponTrace,
            0.01f,     // Every 0.01 seconds (adjust as needed)
            true       // Loop
        );
    }
    else if (NotifyTag == FGameplayTag::RequestGameplayTag(FName("Attack.Damage.Notify.End")))
    {
        // Stop tracing by clearing the timer
        GetWorldTimerManager().ClearTimer(WeaponTraceTimerHandle);
    }
}

/**
 * Update the in-game HUD with the current player status.
 * This function is called to update the in-game HUD with the current player status.
*/
void APGAS_PlayerCharacter::UpdateInGameHUD()
{
    if (MyPlayerHUD)
    {
        MyPlayerHUD->GetInGameHUDWidget()->UpdateHealthValue(GetHealth(), GetMaxHealth());
        MyPlayerHUD->GetInGameHUDWidget()->UpdateStaminaValue(GetStamina(), GetMaxStamina());
        MyPlayerHUD->GetInGameHUDWidget()->UpdateAdrenalineValue(GetAdrenaline(), GetMaxAdrenaline());
        // MyPlayerHUD->GetInGameHUDWidget()->UpdateExperienceValue(GetExperiencePoints(), GetMaxExperiencePoints());
    }
}

