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
 * Date: 9-20-2025
 * =============================================================================
 * PGAS_ArrowProjectile.h
 * Defines the arrow projectile fired from a bow. Handles movement, collision,
 * and applies a GAS damage effect on hit.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "PGAS_ArrowProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UGameplayEffect;

/**
 * Arrow projectile actor.
 * - Moves using ProjectileMovementComponent.
 * - Detects collision via SphereComponent.
 * - Applies a GameplayEffect to the hit target (damage).
 */
UCLASS(Blueprintable, BlueprintType)
class PARAGONGAS_API APGAS_ArrowProjectile : public AActor
{
    GENERATED_BODY()

public:
    /*
    * Functions
    */

    APGAS_ArrowProjectile();

    /** Sets the base damage externally (e.g., from bow charge ratio) */
    UFUNCTION(BlueprintCallable, Category = "Projectile|Damage")
    void SetBaseDamage(float InDamage) { BaseDamage = InDamage; }

    /*
    * Properties
    */

    /** Gameplay effect to apply on hit (e.g., DamageEffect) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Damage")
    TSubclassOf<UGameplayEffect> DamageEffect;

    /** Gameplay tag used when sending the hit event (default = Combat.Damage.Event.Ranged) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Damage")
    FGameplayTag DamageEventTag;

    /** Base damage value (can be overridden or scaled by charge ratio) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Damage")
    float BaseDamage = 10.f;

    /** Whether arrow should be destroyed after hit */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    bool bDestroyOnHit = true;

protected:
    /*
    * Components
    */

    /** Collision sphere for detecting hits */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<USphereComponent> CollisionComp;

    /** Handles movement and ballistics */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    /** Visual mesh (optional) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<UStaticMeshComponent> ArrowMesh;

    /*
    * Functions
    */

    virtual void BeginPlay() override;

private:
    /*
    * Functions
    */

    /** Handle collision events */
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
