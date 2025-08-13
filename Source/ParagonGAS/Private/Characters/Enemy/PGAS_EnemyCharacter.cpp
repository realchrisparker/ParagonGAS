
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
* PGAS_EnemyCharacter.h
* This file defines the enemy character class in the game.
*/

#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <GAS/Abilities/PGAS_ReceiveMeleeDamageAbility.h>

APGAS_EnemyCharacter::APGAS_EnemyCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Set up the character's attribute set.
    // This is where you define your character's attributes like health, mana, etc.
    AttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("AttributeSet"));

    // Default character level
    SetCharacterLevel(1);

    // Add default gameplay tags.
    SetupDefaultGameplayTags();
}

// Called when the game starts or when spawned
void APGAS_EnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Apply default attribute effects.
    ApplyDefaultAttributeEffects();

    // Set up default abilities for the enemy character.
    SetupDefaultAbilities();

    // Equip the default weapon.
    EquipDefaultWeapon();
}

// Called every frame
void APGAS_EnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

/*
 * SetupDefaultGameplayTags function to set up default gameplay tags for this character.
 * This is typically called in the constructor or BeginPlay.
*/
void APGAS_EnemyCharacter::SetupDefaultGameplayTags()
{
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Type.Enemy")));
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Alive")));
    AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Movement.Status.CanMove")));
}

// Handles changes to the character's health attribute
void APGAS_EnemyCharacter::HandleHealthChange(float DeltaValue, AActor* Causer)
{
    OnHealthChanged.Broadcast(DeltaValue, Causer);

    if (GetHealth() <= 0)
    {
        // If the character's health is zero or less, trigger the death event
        OnDeath.Broadcast();
    }
}

/**
 * Equips the default weapon for this character. (Spawn)
 */
void APGAS_EnemyCharacter::EquipDefaultWeapon()
{
    // TODO: Implement inventory system (Change this to work with inventory in the future)
    if (EquippedWeapon || !DefaultWeaponClass) return;

    UWorld* World = GetWorld();
    if (!World || !GetMesh()) return;

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = this;

    APGAS_WeaponBase* NewWeapon = World->SpawnActor<APGAS_WeaponBase>(DefaultWeaponClass, FTransform::Identity, Params);
    if (!NewWeapon) return;

    NewWeapon->EquipToMesh(GetMesh());
    EquippedWeapon = NewWeapon;
}

/**
 * Get the location of the pawn's view.
 */
FVector APGAS_EnemyCharacter::GetPawnViewLocation() const
{
    if (AISightSocketName != NAME_None)
    {
        if (GetMesh() && GetMesh()->DoesSocketExist(AISightSocketName))
        {
            return GetMesh()->GetSocketLocation(AISightSocketName);
        }
    }

    // Fallback to default capsule location if socket missing
    return Super::GetPawnViewLocation();
}

void APGAS_EnemyCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const 
{
    // Location: from the socket if available (keeps Step 1 behavior)
    if (const USkeletalMeshComponent* MeshComp = GetMesh())
    {
        if (AISightSocketName != NAME_None && MeshComp->DoesSocketExist(AISightSocketName))
        {
            OutLocation = MeshComp->GetSocketLocation(AISightSocketName);
        }
        else
        {
            OutLocation = Super::GetPawnViewLocation();
        }
    }
    else
    {
        OutLocation = Super::GetPawnViewLocation();
    }

    // Rotation: force a stable, forward-facing yaw; flatten pitch/roll
    FRotator BaseRot;
    if (bUseControllerYawForSight && Controller)
    {
        BaseRot = Controller->GetControlRotation();
    }
    else
    {
        BaseRot = GetActorRotation();
    }

    BaseRot.Pitch = 0.f;
    BaseRot.Roll = 0.f;
    OutRotation = BaseRot;

    // Debug line
    if (bDebugDrawSightLine)
    {
        const FVector End = OutLocation + OutRotation.Vector() * 300.f; // adjustable length
        DrawDebugLine(GetWorld(), OutLocation, End, FColor::Green, false, -1.f, 0, 2.f);
        DrawDebugSphere(GetWorld(), End, 8.f, 8, FColor::Red, false, -1.f);
    }
}