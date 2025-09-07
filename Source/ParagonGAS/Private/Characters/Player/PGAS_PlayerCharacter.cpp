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
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include <GAS/Abilities/PGAS_GameplayAbility_Montage.h>
#include <GAS/Effects/PGAS_GE_InstantStaminaReduction.h>
#include <GAS/Effects/PGAS_GE_InfiniteStaminaReduction.h>
#include <Data/PGAS_EventAdditionalData.h>
#include <Enums/PGAS_Direction.h>
#include <Animations/Base/PGAS_AnimInstanceBase.h>


// Sets default values
APGAS_PlayerCharacter::APGAS_PlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // --- Camera Boom (SpringArm) ---
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f; // typical Souls-like distance
    CameraBoom->SocketOffset = FVector(0.f, 50.f, 60.f); // slight offset to the right & above
    CameraBoom->bUsePawnControlRotation = true; // boom rotates with controller
    CameraBoom->bInheritPitch = true;
    CameraBoom->bInheritYaw = true;
    CameraBoom->bInheritRoll = true;
    CameraBoom->bDoCollisionTest = true; // prevent camera from clipping walls

    // --- Follow Camera ---
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false; // camera does NOT rotate with pawn
    FollowCamera->FieldOfView = 95.f; // slightly wide FOV for combat readability

    // --- Character Movement ---
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;  // pawn faces camera yaw
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false; // disables auto-facing input direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f); // slower, camera-driven rotation
    GetCharacterMovement()->bUseControllerDesiredRotation = true; // match facing with camera yaw

    // Create the combat core component
    CombatCoreComponent = CreateDefaultSubobject<UPGAS_CombatCoreComponent>(TEXT("Combat Core Component"));

    // Create the hitbox component.
    HitboxComponent = CreateDefaultSubobject<UPGAS_HitboxComponent>(TEXT("Hitbox Component"));

    // Create the lock-on component.
    LockOnComponent = CreateDefaultSubobject<UPGAS_LockOnComponent>(TEXT("Lock On Component"));

    // Create the block component.
    BlockComponent = CreateDefaultSubobject<UPGAS_BlockComponent>(TEXT("Block Component"));

    // Create the dodge component.
    DodgeComponent = CreateDefaultSubobject<UPGAS_DodgeComponent>(TEXT("Dodge Component"));

    /**
     * Set up the character's attribute set.
     * This is where you define your character's attributes like health, mana, etc.
     * Unreal Engine will automatically replicate this attribute set to the client.
     * Unreal Engine GAS will automatically load this attribute set when the Ability System Component is created.
     */
    AttributeSet = CreateDefaultSubobject<UPlayerCharacterAttributeSet>(TEXT("AttributeSet"));

    // Default character level
    SetCharacterLevel(1);
}

// Called when the game starts or when spawned
void APGAS_PlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    TObjectPtr<APGAS_PlayerController> PC = Cast<APGAS_PlayerController>(GetController());
    if (PC)
    {
        // PlayerState and HUD may not always be valid in BeginPlay due to spawn order/networking; safe-guard accordingly.
        TObjectPtr<APGAS_PlayerState> FoundPlayerState = Cast<APGAS_PlayerState>(PC->GetPlayerState<APlayerState>());
        if (FoundPlayerState)
        {
            MyPlayerState = FoundPlayerState;
        }

        // Set HUD reference.
        TObjectPtr<APGAS_HUD> FoundHUD = Cast<APGAS_HUD>(PC->GetHUD());
        if (FoundHUD)
        {
            MyPlayerHUD = FoundHUD;
        }
    }

    // Make sure the character always uses LOD0.
    if (USkeletalMeshComponent* MyMesh = GetMesh())
    {
        MyMesh->SetForcedLOD(1); // If visual fidelity is critical for the player
    }

    // Set max speed here
    if (UCharacterMovementComponent* CharMove = GetCharacterMovement())
    {
        if (!FMath::IsNearlyEqual(CharMove->MaxWalkSpeed, 350.f))
            CharMove->MaxWalkSpeed = 350.f;
    }

    // Set up the initial HUD values
    if (MyPlayerHUD)
    {
        MyPlayerHUD->GetInGameHUDWidget()->UpdateHealthValue(GetHealth(), GetMaxHealth());
        MyPlayerHUD->GetInGameHUDWidget()->UpdateStaminaValue(GetStamina(), GetMaxStamina());
        MyPlayerHUD->GetInGameHUDWidget()->UpdateAdrenalineValue(GetAdrenaline(), GetMaxAdrenaline());
    }

    // Bind to attribute set stamina change callback
    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
    {
        if (AttributeSet)
        {
            // Bind our function to the delegate for the Health attribute
            ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(
                this, &APGAS_PlayerCharacter::OnHealthAttributeChanged);
            // Bind our function to the delegate for the Stamina attribute
            ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(
                this, &APGAS_PlayerCharacter::OnStaminaAttributeChanged);
            // Bind our function to the delegate for the Adrenaline attribute
            ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetAdrenalineAttribute()).AddUObject(
                this, &APGAS_PlayerCharacter::OnAdrenalineAttributeChanged);
        }
    }

    // Bind CombatCore -> windowing events.
    if (UPGAS_CombatCoreComponent* Core = GetCombatCoreComponent())
    {
        Core->OnCombatWindowOpen.AddDynamic(this, &APGAS_PlayerCharacter::HandleCombatWindowStartHanded);
        Core->OnCombatWindowClose.AddDynamic(this, &APGAS_PlayerCharacter::HandleCombatWindowEndHanded);
    }

    // Bind hitbox events.
    if (UPGAS_HitboxComponent* HB = GetHitboxComponent())
    {
        HB->OnHitboxHit.AddDynamic(this, &APGAS_PlayerCharacter::HandleHitboxHit);
    }

    // Bind lockon events.
    if (UPGAS_LockOnComponent* LockOn = GetLockOnComponent())
    {
        // LockOn->OnLocked.AddDynamic(this, &APGAS_PlayerCharacter::HandleLockOnTargetChanged);
        // LockOn->OnUnlocked.AddDynamic(this, &APGAS_PlayerCharacter::HandleLockOnTargetChanged);
        // LockOn->OnTargetChanged.AddDynamic(this, &APGAS_PlayerCharacter::HandleLockOnTargetChanged);
    }
}

void APGAS_PlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
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

// --------------------
// Input Functions
// --------------------

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
            EnhancedInputComp->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APGAS_PlayerCharacter::JumpReleaseAction);
        }
        if (IA_Sprint)
        {
            EnhancedInputComp->BindAction(IA_Sprint, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::SprintStartAction);
            EnhancedInputComp->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &APGAS_PlayerCharacter::SprintReleaseAction);
        }
        if (IA_LightAttack)
        {
            EnhancedInputComp->BindAction(IA_LightAttack, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::LightAttackAction);
        }
        if (IA_HeavyAttack)
        {
            EnhancedInputComp->BindAction(IA_HeavyAttack, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::HeavyAttackAction);
        }
        if (IA_LeftHandLightAttack)
        {
            EnhancedInputComp->BindAction(IA_LeftHandLightAttack, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::LeftHandLightAttackAction);
        }
        if (IA_LeftHandHeavyAttack)
        {
            EnhancedInputComp->BindAction(IA_LeftHandHeavyAttack, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::LeftHandHeavyAttackAction);
        }
        if (IA_RightHandLightAttack)
        {
            EnhancedInputComp->BindAction(IA_RightHandLightAttack, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::RightHandLightAttackAction);
        }
        if (IA_RightHandHeavyAttack)
        {
            EnhancedInputComp->BindAction(IA_RightHandHeavyAttack, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::RightHandHeavyAttackAction);
        }
        if (IA_Block)
        {
            EnhancedInputComp->BindAction(IA_Block, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::BlockStartedAction);
            EnhancedInputComp->BindAction(IA_Block, ETriggerEvent::Completed, this, &APGAS_PlayerCharacter::BlockReleaseAction);
        }
        if (IA_LockOn)
        {
            EnhancedInputComp->BindAction(IA_LockOn, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::LockOnAction);
        }
        if (IA_Dodge)
        {
            EnhancedInputComp->BindAction(IA_Dodge, ETriggerEvent::Started, this, &APGAS_PlayerCharacter::DodgeAction);
        }
        if (IA_Roll)
        {
            // Set up as a double tap so we don't want to use Started here, double tap detection fires on Triggered. Started will fire on one tap.
            EnhancedInputComp->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &APGAS_PlayerCharacter::RollAction);
        }
        if (IA_RollX)
        {
            EnhancedInputComp->BindAction(IA_RollX, ETriggerEvent::Triggered, this, &APGAS_PlayerCharacter::RollAction);
        }
        if (IA_RollY)
        {
            EnhancedInputComp->BindAction(IA_RollY, ETriggerEvent::Triggered, this, &APGAS_PlayerCharacter::RollAction);
        }
    }
}

/*
 * MoveStartedAction function to handle the start of movement input.
 * This function is called when the IA_Move input action is started.
*/
void APGAS_PlayerCharacter::MoveStartedAction(const FInputActionInstance& Value)
{
    if (HasGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove"))) == false)
    {
        return;
    }
    
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Jogging"))); // This tag indicates the character is jogging or moving.
}

/*
 * Move function to handle movement input.
 * This function is called when the IA_Move input action is triggered.
*/
void APGAS_PlayerCharacter::MoveAction(const FInputActionInstance& Value)
{
    if (!HasGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove"))))
    {
        return;
    }

    const FVector2D MovementVector = Value.GetValue().Get<FVector2D>();

    if (!Controller || MovementVector.IsNearlyZero())
    {
        return;
    }

    // Apply movement input relative to camera yaw
    const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

    if (FMath::Abs(MovementVector.Y) > 0.0f)
    {
        const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(ForwardDir, MovementVector.Y);
    }

    if (FMath::Abs(MovementVector.X) > 0.0f)
    {
        const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(RightDir, MovementVector.X);
    }

    // Reset idle state when moving
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
    if (!Controller) return;

    // IA_Look is set up to provide a 2D axis (X = turn, Y = look up/down).
    const FVector2D LookAxisValue = Value.Get<FVector2D>();

    // --- Camera Control ---

    AddControllerYawInput(LookAxisValue.X); // Add yaw input (horizontal turn).    
    AddControllerPitchInput(LookAxisValue.Y); // Add pitch input (vertical look).

    // --- Stick Flick Detection ---
    const float CurrentTime = GetWorld()->GetTimeSeconds();

    if (UPGAS_LockOnComponent* LockComp = GetLockOnComponent())
    {
        if (LockComp->IsLockedOn())
        {
            // Check X axis for flick (ignore mouse since it won't usually hit threshold)
            if (bStickAtRest && FMath::Abs(LookAxisValue.X) >= StickFlickThreshold)
            {
                if (CurrentTime - LastFlickTime >= FlickCooldown)
                {
                    if (LookAxisValue.X > 0.f)
                    {
                        LockComp->SwitchTargetRight();
                    }
                    else
                    {
                        LockComp->SwitchTargetLeft();
                    }

                    LastFlickTime = CurrentTime;
                    bStickAtRest = false; // must return to rest before next flick
                }
            }
            else if (FMath::Abs(LookAxisValue.X) < 0.2f)
            {
                // Stick returned near center → ready to detect next flick
                bStickAtRest = true;
            }
        }
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
    // Make sure character can Jump.
    if (HasGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove"))) == false)
    {
        return;
    }

    // Early-out if we're already falling (i.e., in the air).
    if (GetCharacterMovement() && GetCharacterMovement()->IsFalling())
    {
        return;
    }

    // Activate by tag
    static FGameplayTag JumpAbilityTag = FGameplayTag::RequestGameplayTag(FName("Character.Ability.Jump"));
    ActivateAbilitiesWithTags(FGameplayTagContainer(JumpAbilityTag), true);

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * JumpReleaseAction function to handle the release of jumping input.
 * This function is called when the IA_Jump input action is released.
*/
void APGAS_PlayerCharacter::JumpReleaseAction(const FInputActionValue& Value)
{
    if (GetAbilitySystemComponent())
    {
        // GAS: Tell the ASC that the input was released for this ability (input ID is usually 0 for jump)
        GetAbilitySystemComponent()->AbilityLocalInputReleased(0); // 0 is the default input ID, use your actual mapping if needed
    }

    // Reset idle time and animation flag when movement stops
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * Sprint function to handle sprinting input.
 * This function is called when the IA_Sprint input action is triggered.
*/
void APGAS_PlayerCharacter::SprintStartAction(const FInputActionValue& Value)
{
    // Activate by input ID
    GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<int32>(EPGAS_AbilityInputID::Sprint));

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * SprintReleaseAction function to handle the release of sprinting input.
 * This function is called when the IA_Sprint input action is released.
*/
void APGAS_PlayerCharacter::SprintReleaseAction(const FInputActionValue& Value)
{
    if (GetAbilitySystemComponent())
    {
        // Release sprint input
        GetAbilitySystemComponent()->AbilityLocalInputReleased(static_cast<int32>(EPGAS_AbilityInputID::Sprint));
    }

    // Reset idle time and animation flag when movement stops
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * MoveStartedAction function to handle the start of movement input.
 * This function is called when the IA_Move input action is started.
*/
void APGAS_PlayerCharacter::BlockStartedAction(const FInputActionInstance& Value)
{
    if (UPGAS_BlockComponent* BlockComp = GetBlockComponent())
    {
        BlockComp->StartBlock();
    }

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * BlockReleaseAction function to handle the release of blocking input.
 * This function is called when the IA_Block input action is released.
*/
void APGAS_PlayerCharacter::BlockReleaseAction(const FInputActionValue& Value)
{
    if (UPGAS_BlockComponent* BlockComp = GetBlockComponent())
    {
        BlockComp->StopBlock();
    }

    // Reset idle time and animation flag when movement stops
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * LightAttackAction function to handle left hand attack input.
 * This function is called when the IA_LightAttack input action is triggered.
*/
void APGAS_PlayerCharacter::LightAttackAction(const FInputActionValue& Value)
{
    // Get the combat core component
    if (UPGAS_CombatCoreComponent* CoreComponent = GetCombatCoreComponent())
    {
        const FPGAS_AttackType Attack = CoreComponent->GetAttackByTypeAndIndex(EPGAS_WeaponAttackType::Light, CoreComponent->CurrentComboIndex);

        // Get the first attack of the specified type
        if (Attack.AbilityTag.IsValid()) {
            const bool bActivated = CoreComponent->ActivateAbilityByTag(Attack.AbilityTag); // Activate the ability
            SetIsAttacking(bActivated); // Set the attacking flag to true
        }
    }

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * HeavyAttackAction function to handle left hand attack input.
 * This function is called when the IA_HeavyAttack input action is triggered.
*/
void APGAS_PlayerCharacter::HeavyAttackAction(const FInputActionValue& Value)
{
    // Get the combat core component
    if (UPGAS_CombatCoreComponent* CoreComponent = GetCombatCoreComponent())
    {
        const FPGAS_AttackType Attack = CoreComponent->GetAttackByTypeAndIndex(EPGAS_WeaponAttackType::Heavy, CoreComponent->CurrentComboIndex);

        // Get the first attack of the specified type
        if (Attack.AbilityTag.IsValid()) {
            const bool bActivated = CoreComponent->ActivateAbilityByTag(Attack.AbilityTag); // Activate the ability
            SetIsAttacking(bActivated); // Set the attacking flag to true
        }
    }

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * LeftHandLightAttackAction function to handle left hand attack input.
 * This function is called when the IA_LeftHandLightAttack input action is triggered.
*/
void APGAS_PlayerCharacter::LeftHandLightAttackAction(const FInputActionValue& Value)
{
    if (UPGAS_CombatCoreComponent* CoreComponent = GetCombatCoreComponent())
    {
        // Get the first attack of the specified type
        const FPGAS_AttackType Attack = CoreComponent->GetAttackByTypeAndHand(EPGAS_WeaponAttackType::Light, EPGAS_WeaponHand::Left);
        if (Attack.AbilityTag.IsValid())
        {
            const bool bActivated = CoreComponent->ActivateAbilityByTag(Attack.AbilityTag); // Activate the ability
            SetIsAttacking(bActivated); // Set the attacking flag to true
        }
    }

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/*
 * LeftHandHeavyAttackAction function to handle left hand attack input.
 * This function is called when the IA_LeftHandHeavyAttack input action is triggered.
*/
void APGAS_PlayerCharacter::LeftHandHeavyAttackAction(const FInputActionValue& Value)
{
    if (UPGAS_CombatCoreComponent* CoreComponent = GetCombatCoreComponent())
    {
        // Get the first attack of the specified type
        const FPGAS_AttackType Attack = CoreComponent->GetAttackByTypeAndHand(EPGAS_WeaponAttackType::Heavy, EPGAS_WeaponHand::Left);
        if (Attack.AbilityTag.IsValid())
        {
            const bool bActivated = CoreComponent->ActivateAbilityByTag(Attack.AbilityTag); // Activate the ability
            SetIsAttacking(bActivated); // Set the attacking flag to true
        }
    }

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/**
 * RightHandLightAttackAction function to handle right hand attack input.
 * This function is called when the IA_RightHandLightAttack input action is triggered.
 */
void APGAS_PlayerCharacter::RightHandLightAttackAction(const FInputActionValue& Value)
{
    if (UPGAS_CombatCoreComponent* CoreComponent = GetCombatCoreComponent())
    {
        // Get the first attack of the specified type
        const FPGAS_AttackType Attack = CoreComponent->GetAttackByTypeAndHand(EPGAS_WeaponAttackType::Light, EPGAS_WeaponHand::Right);
        if (Attack.AbilityTag.IsValid())
        {
            const bool bActivated = CoreComponent->ActivateAbilityByTag(Attack.AbilityTag); // Activate the ability
            SetIsAttacking(bActivated); // Set the attacking flag to true
        }
    }

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/**
 * RightHandHeavyAttackAction function to handle right hand attack input.
 * This function is called when the IA_RightHandHeavyAttack input action is triggered.
 */
void APGAS_PlayerCharacter::RightHandHeavyAttackAction(const FInputActionValue& Value)
{
    if (UPGAS_CombatCoreComponent* CoreComponent = GetCombatCoreComponent())
    {
        // Get the first attack of the specified type
        const FPGAS_AttackType Attack = CoreComponent->GetAttackByTypeAndHand(EPGAS_WeaponAttackType::Heavy, EPGAS_WeaponHand::Right);
        if (Attack.AbilityTag.IsValid())
        {
            const bool bActivated = CoreComponent->ActivateAbilityByTag(Attack.AbilityTag); // Activate the ability
            SetIsAttacking(bActivated); // Set the attacking flag to true
        }
    }

    // Reset idle time and animation flag when movement starts
    IdleTime = 0.f;
    bIdleAnimationPlayed = false;
}

/**
 * LockOnAction function to handle lock-on input.
 * This function is called when the IA_LockOn input action is triggered.
 */
void APGAS_PlayerCharacter::LockOnAction(const FInputActionValue& Value)
{
    if (UPGAS_LockOnComponent* LockComp = FindComponentByClass<UPGAS_LockOnComponent>())
    {
        if (LockComp->IsLockedOn())
        {
            // Already locked, so toggle off
            LockComp->Unlock();
        }
        else
        {
            // Try to lock onto nearest target
            LockComp->TryLockOnNearest();
        }
    }
}

/**
 * Called by the IA_Dodge input action to handle dodging.
 */
void APGAS_PlayerCharacter::DodgeAction(const FInputActionInstance& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("DodgeAction: Dodge input received"));
    if (UPGAS_DodgeComponent* DodgeComp = GetDodgeComponent())
    {
        EPGAS_DodgeDirection ChosenDirection = EPGAS_DodgeDirection::Backward; // Default to backward dodge
        UPGAS_AnimInstanceBase* AnimInstance = Cast<UPGAS_AnimInstanceBase>(GetMesh()->GetAnimInstance());
        if (AnimInstance) // Check if the animation instance is valid
        {
            // Convert animation direction to dodge direction
            EPGAS_Direction eDirection = AnimInstance->eDirection;
            switch (eDirection)
            {
            case EPGAS_Direction::Forward:
                ChosenDirection = EPGAS_DodgeDirection::Forward;
                break;
            case EPGAS_Direction::Backward:
                ChosenDirection = EPGAS_DodgeDirection::Backward;
                break;
            case EPGAS_Direction::Left:
                ChosenDirection = EPGAS_DodgeDirection::Left;
                break;
            case EPGAS_Direction::Right:
                ChosenDirection = EPGAS_DodgeDirection::Right;
                break;
            default:
                ChosenDirection = EPGAS_DodgeDirection::Backward;
                break;
            }
        }

        // Log chosen direction
        UE_LOG(LogTemp, Warning, TEXT("DodgeAction: Performing %s dodge"), *UEnum::GetValueAsString(ChosenDirection));

        // Lookup dodge data
        FPGAS_DodgeType Dodge = DodgeComp->GetDodgeByCategoryAndDirection(EPGAS_DodgeCategory::Dodge, ChosenDirection);

        // Trigger dodge if valid
        if (Dodge.Montage)
        {
            DodgeComp->PerformDodge(Dodge);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("DodgeAction: No montage found for %s dodge"), *UEnum::GetValueAsString(ChosenDirection));
        }
    }
}

/**
 * Called by the IA_Roll input action to handle rolling.
 */
void APGAS_PlayerCharacter::RollAction(const FInputActionInstance& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("RollAction: Roll input received"));

    FVector2D StickInput = FVector2D::ZeroVector;

    FInputActionValue ActionValue = Value.GetValue();
    switch (ActionValue.GetValueType())
    {
        case EInputActionValueType::Axis2D: // Gamepad stick
            {
                StickInput = ActionValue.Get<FVector2D>();
                break;
            }
        case EInputActionValueType::Axis1D: // Keyboard WASD (split into RollX and RollY)
            {
                float AxisValue = ActionValue.Get<float>();

                if (Value.GetSourceAction() == IA_RollX)
                {
                    StickInput = FVector2D(AxisValue, 0.f);
                }
                else if (Value.GetSourceAction() == IA_RollY)
                {
                    StickInput = FVector2D(0.f, AxisValue);
                }
                break;
            }
        default:
            break;
    }

    UE_LOG(LogTemp, Warning, TEXT("RollAction: StickInput X=%f, Y=%f"), StickInput.X, StickInput.Y);

    if (StickInput.IsNearlyZero())
    {
        UE_LOG(LogTemp, Warning, TEXT("RollAction: No stick input detected"));
        return;
    }

    // Normalize and resolve closest cardinal direction
    StickInput.Normalize();

    FVector2D Forward(0.f, 1.f);
    FVector2D Back(0.f, -1.f);
    FVector2D Right(1.f, 0.f);
    FVector2D Left(-1.f, 0.f);

    float DotForward = FVector2D::DotProduct(StickInput, Forward);
    float DotBack = FVector2D::DotProduct(StickInput, Back);
    float DotRight = FVector2D::DotProduct(StickInput, Right);
    float DotLeft = FVector2D::DotProduct(StickInput, Left);

    float MaxDot = DotForward;
    EPGAS_DodgeDirection ChosenDirection = EPGAS_DodgeDirection::Forward;

    if (DotBack > MaxDot) { MaxDot = DotBack;  ChosenDirection = EPGAS_DodgeDirection::Backward; }
    if (DotRight > MaxDot) { MaxDot = DotRight; ChosenDirection = EPGAS_DodgeDirection::Right; }
    if (DotLeft > MaxDot) { MaxDot = DotLeft;  ChosenDirection = EPGAS_DodgeDirection::Left; }

    UE_LOG(LogTemp, Warning, TEXT("RollAction: Performing %s roll"), *UEnum::GetValueAsString(ChosenDirection));

    if (UPGAS_DodgeComponent* DodgeComp = GetDodgeComponent())
    {
        FPGAS_DodgeType Roll = DodgeComp->GetDodgeByCategoryAndDirection(
            EPGAS_DodgeCategory::Roll,
            ChosenDirection
        );

        if (Roll.Montage)
        {
            DodgeComp->PerformDodge(Roll);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("RollAction: No montage found for %s roll"),
                *UEnum::GetValueAsString(ChosenDirection));
        }
    }

    // UE_LOG(LogTemp, Warning, TEXT("RollAction: Roll input received"));

    // if (UPGAS_DodgeComponent* DodgeComp = GetDodgeComponent())
    // {
    //     const EInputActionValueType type = Value.GetSourceAction()->ValueType;
    //     UE_LOG(LogTemp, Warning, TEXT("RollAction: StickInput Type=%s"), *UEnum::GetValueAsString(type));
    //     FVector2D StickInput = Value.GetValue().Get<FVector2D>();
    //     UE_LOG(LogTemp, Warning, TEXT("RollAction: StickInput X=%f, Y=%f"), StickInput.X, StickInput.Y);

    //     if (StickInput.IsNearlyZero())
    //     {
    //         UE_LOG(LogTemp, Warning, TEXT("RollAction: No stick input detected"));
    //         return;
    //     }

    //     // Normalize input
    //     StickInput.Normalize();

    //     // Reference directions in 2D
    //     FVector2D Forward(0.f, 1.f);
    //     FVector2D Back(0.f, -1.f);
    //     FVector2D Right(1.f, 0.f);
    //     FVector2D Left(-1.f, 0.f);

    //     // Dot products
    //     float DotForward = FVector2D::DotProduct(StickInput, Forward);
    //     float DotBack = FVector2D::DotProduct(StickInput, Back);
    //     float DotRight = FVector2D::DotProduct(StickInput, Right);
    //     float DotLeft = FVector2D::DotProduct(StickInput, Left);

    //     // Pick max
    //     float MaxDot = DotForward;
    //     EPGAS_DodgeDirection ChosenDirection = EPGAS_DodgeDirection::Forward;

    //     if (DotBack > MaxDot) { MaxDot = DotBack; ChosenDirection = EPGAS_DodgeDirection::Backward; }
    //     if (DotRight > MaxDot) { MaxDot = DotRight; ChosenDirection = EPGAS_DodgeDirection::Right; }
    //     if (DotLeft > MaxDot) { MaxDot = DotLeft; ChosenDirection = EPGAS_DodgeDirection::Left; }

    //     UE_LOG(LogTemp, Warning, TEXT("RollAction: Performing %s roll"),
    //         *UEnum::GetValueAsString(ChosenDirection));

    //     // Lookup roll
    //     FPGAS_DodgeType Roll = DodgeComp->GetDodgeByCategoryAndDirection(
    //         EPGAS_DodgeCategory::Roll,
    //         ChosenDirection
    //     );

    //     if (Roll.Montage)
    //     {
    //         DodgeComp->PerformDodge(Roll);
    //     }
    //     else
    //     {
    //         UE_LOG(LogTemp, Warning, TEXT("RollAction: No montage found for %s roll"),
    //             *UEnum::GetValueAsString(ChosenDirection));
    //     }
    // }
}

// --------------------
// GAS Functions
// --------------------

/*
 * Gives the player character default abilities.
 * This function overrides the base class implementation to provide custom functionality. 
 * @note It calls the base class's SetupDefaultAbilities function to ensure any inherited functionality is executed.
*/
void APGAS_PlayerCharacter::SetupDefaultAbilities()
{
    // Call the base class implementation to ensure any inherited functionality is executed.
    Super::SetupDefaultAbilities();

    // Optional, add player character specific default abilities here.
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

/**
 * Called when the health attribute changes.
 * @param Data The data about the attribute change.
*/
void APGAS_PlayerCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
    // The Data parameter contains the new and old values
    float NewValue = Data.NewValue;

    if (MyPlayerHUD)
    {
        MyPlayerHUD->UpdateHealthBar(NewValue, GetMaxHealth());
    }
}

/**
 * Called when the stamina attribute changes.
 * @param Data The data about the attribute change.
*/
void APGAS_PlayerCharacter::OnStaminaAttributeChanged(const FOnAttributeChangeData& Data)
{
    // The Data parameter contains the new and old values
    float NewValue = Data.NewValue;

    if (MyPlayerHUD)
    {
        MyPlayerHUD->UpdateStaminaBar(NewValue, GetMaxStamina());
    }
}

/**
 * Called when the adrenaline attribute changes.
 * @param Data The data about the attribute change.
*/
void APGAS_PlayerCharacter::OnAdrenalineAttributeChanged(const FOnAttributeChangeData& Data)
{
    // The Data parameter contains the new and old values
    float NewValue = Data.NewValue;

    if (MyPlayerHUD)
    {
        MyPlayerHUD->UpdateAdrenalineBar(NewValue, GetMaxAdrenaline());
    }
}

// --------------------
// Windowing (Combat)
// --------------------

/**
 * Called when a combat window is started
 * This function is responsible for starting hit detection in the hitbox component.
 */
void APGAS_PlayerCharacter::HandleCombatWindowStartHanded(FPGAS_AttackType AttackData)
{
    UE_LOG(LogTemp, Warning, TEXT("APGAS_PlayerCharacter::HandleCombatWindowStartHanded: CombatTag=%s, Hand=%s"), *AttackData.AbilityTag.ToString(), *UEnum::GetValueAsString(AttackData.Hand));

    if (!HasAuthority()) return; // exit if no authority

    if (UPGAS_HitboxComponent* Component = GetHitboxComponent())
    {
        // Forward the gameplay tag + hand into Hitbox
        Component->StartHitDetection(AttackData);
    }
}

/**
 * Called when a combat window is ended
 * This function is responsible for stopping hit detection in the hitbox component.
 */
void APGAS_PlayerCharacter::HandleCombatWindowEndHanded(FPGAS_AttackType AttackData)
{
    UE_LOG(LogTemp, Warning, TEXT("APGAS_PlayerCharacter::HandleCombatWindowEndHanded: CombatTag=%s, Hand=%s"), *AttackData.AbilityTag.ToString(), *UEnum::GetValueAsString(AttackData.Hand));

    if (!HasAuthority()) return; // exit if no authority

    if (UPGAS_HitboxComponent* Component = GetHitboxComponent())
    {
        Component->StopHitDetection();
    }
}

/**
 * Handle hit events from the hitbox component.
 * We use this to apply damage to the hit actor.
 */
void APGAS_PlayerCharacter::HandleHitboxHit(AActor* HitActor, const FHitResult& Hit, FPGAS_AttackType AttackType)
{
    if (!HitActor || !GetAbilitySystemComponent()) return;

    // Play FX & Sound from the Weapon DataAsset
    if (AttackType.WeaponData)
    {
        if (AttackType.WeaponData->HitImpactFX)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                AttackType.WeaponData->HitImpactFX,
                Hit.ImpactPoint,
                Hit.ImpactNormal.Rotation()
            );
        }

        if (AttackType.WeaponData->HitImpactSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                GetWorld(),
                AttackType.WeaponData->HitImpactSound,
                Hit.ImpactPoint
            );
        }
    }

    // Get target ASC
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
    if (!TargetASC) return;

    // Create effect context (contains instigator, source object, hit result)
    FGameplayEffectContextHandle Context = GetAbilitySystemComponent()->MakeEffectContext();
    Context.AddSourceObject(this);
    Context.AddHitResult(Hit);

    // Use the damage effect from AttackType (defaults to PGAS_GE_MeleeDamageEffect)
    if (AttackType.DamageEffect)
    {
        FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(AttackType.DamageEffect, 1.f, Context);

        if (SpecHandle.IsValid())
        {
            // SetByCaller magnitude if you want attacks to vary in damage
            static FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Melee"));
            SpecHandle.Data->SetSetByCallerMagnitude(DamageTag, AttackType.WeaponData->BaseDamage > 0 ? AttackType.WeaponData->BaseDamage : 1.f);

            GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
        }
    }
}
