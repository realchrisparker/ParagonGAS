 /*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * This ability listens for the Combat.Damage.Event.Melee.Staff tag and handles applying damage.
 * Author: Christopher D. Parker
 * Date: 7-4-2025
 * =============================================================================
 * PGAC_ReceiveStaffMeleeDamageAbility
 * This ability listens for the Combat.Damage.Event.Melee.Staff tag and handles applying damage.
 */

 #pragma once

 #include "CoreMinimal.h"
 #include "Abilities/GameplayAbility.h"
 #include "PGAS_ReceiveStaffMeleeDamageAbility.generated.h"

 UCLASS(meta=(DisplayName="Receive Staff Melee Damage Ability"))
 class PARAGONGAS_API UPGAS_ReceiveStaffMeleeDamageAbility : public UGameplayAbility
 {
     GENERATED_BODY()
 
 public:
     UPGAS_ReceiveStaffMeleeDamageAbility();
 
     /** Activate the ability. */
     virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
         const FGameplayAbilityActorInfo* ActorInfo,
         const FGameplayAbilityActivationInfo ActivationInfo,
         const FGameplayEventData* TriggerEventData) override;
 
     /** End the ability */
     virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
         const FGameplayAbilityActorInfo* ActorInfo,
         const FGameplayAbilityActivationInfo ActivationInfo,
         bool bReplicateEndAbility,
         bool bWasCancelled);

     UPROPERTY(BlueprintReadOnly, Category="Ability|Tags",
         meta=(DisplayName="Event Tag", ToolTip="The event tag that triggers this ability"))
     FGameplayTag EventTriggerTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Melee.Staff"));
 };
  