#include "Components/PGAS_CombatParryComponent.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/Base/PGAS_CharacterBase.h"


// Constructor
UPGAS_CombatParryComponent::UPGAS_CombatParryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPGAS_CombatParryComponent::BeginPlay()
{
    Super::BeginPlay();
}

/**
 * Starts a parry attempt (called when player presses parry input)
 * This will initiate the parry sequence: startup -> active window -> recovery.
 */
void UPGAS_CombatParryComponent::StartParry()
{
    if (bIsParrying) return; // prevent overlapping parries
    bIsParrying = true;

    // --- Activate Parry Ability if valid ---
    if (ParryAbilityTag.IsValid())
    {
        if (APGAS_CharacterBase* OwnerChar = Cast<APGAS_CharacterBase>(GetOwner()))
        {
            if (UAbilitySystemComponent* ASC = OwnerChar->GetAbilitySystemComponent())
            {
                bool bSuccess = ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(ParryAbilityTag), true);
                if(bSuccess)
                {
                    UE_LOG(LogTemp, Warning, TEXT("UPGAS_CombatParryComponent::StartParry - Activated Parry ability with tag %s"), *ParryAbilityTag.ToString());

                    // Add "Parrying" tag to owner
                    OwnerChar->AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("Combat.Parrying")));

                    // Startup phase
                    GetWorld()->GetTimerManager().SetTimer(
                        ParryStartupHandle,
                        this,
                        &UPGAS_CombatParryComponent::EnterActiveWindow,
                        StartupDuration,
                        false
                    );
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("UPGAS_CombatParryComponent::StartParry - Failed to activate Parry ability with tag %s"), *ParryAbilityTag.ToString());
                }
            }
        }
    }
}

/**
 * Called when parry enters active window
 */
void UPGAS_CombatParryComponent::EnterActiveWindow()
{
    bIsInActiveWindow = true;

    // Remove "Parrying" tag from owner
    if (APGAS_CharacterBase* OwnerChar = Cast<APGAS_CharacterBase>(GetOwner()))
    {
        OwnerChar->RemoveGameplayTag(FGameplayTag::RequestGameplayTag(FName("Combat.Parrying")));
    }

    // --- Active parry window ---
    GetWorld()->GetTimerManager().SetTimer(
        ParryActiveHandle,
        this,
        &UPGAS_CombatParryComponent::EnterRecoveryPhase,
        ActiveWindowDuration,
        false
    );
}

/**
 * Called when parry enters recovery phase
 */
void UPGAS_CombatParryComponent::EnterRecoveryPhase()
{
    bIsInActiveWindow = false;

    // Remove "Parrying" tag from owner
    if (APGAS_CharacterBase* OwnerChar = Cast<APGAS_CharacterBase>(GetOwner()))
    {
        OwnerChar->RemoveGameplayTag(FGameplayTag::RequestGameplayTag(FName("Combat.Parrying")));
    }

    // --- Recovery phase ---
    GetWorld()->GetTimerManager().SetTimer(
        ParryRecoveryHandle,
        [ this ] ()
        {
            bIsParrying = false; // reset fully
        },
        RecoveryDuration,
        false
    );
}

/**
 * Called when parry is successful
 * This will immediately transition to recovery phase.
 * @param Attacker The actor who initiated the attack that was parried.
 */
void UPGAS_CombatParryComponent::OnParrySuccess(AActor* Attacker)
{
    // Clear timers and go directly to recovery
    GetWorld()->GetTimerManager().ClearTimer(ParryActiveHandle);
    EnterRecoveryPhase();

    OnParrySuccessDelegate.Broadcast();
}

/**
 * Called when parry fails
 */
void UPGAS_CombatParryComponent::OnParryFailed()
{
    OnParryFailedDelegate.Broadcast();
    EnterRecoveryPhase();
}
