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
 * PGAS_ArrowProjectile.cpp
 * Implementation of the arrow projectile logic.
 */

#include "Weapons/Projectiles/PGAS_ArrowProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/PGAS_AbilitySystemComponent.h"
#include "Data/PGAS_EventAdditionalData.h"

APGAS_ArrowProjectile::APGAS_ArrowProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    // Collision
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->InitSphereRadius(8.f);
    CollisionComp->SetCollisionProfileName("Projectile");
    CollisionComp->SetNotifyRigidBodyCollision(true);
    CollisionComp->SetGenerateOverlapEvents(true);
    RootComponent = CollisionComp;

    // Mesh
    ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
    ArrowMesh->SetupAttachment(RootComponent);
    ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Projectile movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 6000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.5f; // Adjust for arc

    InitialLifeSpan = 10.f; // destroy after 10 seconds

    // Default damage tag
    DamageEventTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Ranged"));
}

void APGAS_ArrowProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (CollisionComp)
    {
        CollisionComp->OnComponentHit.AddDynamic(this, &APGAS_ArrowProjectile::OnHit);
    }
}

void APGAS_ArrowProjectile::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    // Ignore self
    if (!OtherActor || OtherActor == GetOwner())
    {
        return;
    }

    // Only apply effect if we have a valid damage effect and damage > 0
    if (DamageEffect && BaseDamage > 0.f)
    {
        AActor* InstigatorActor = GetOwner(); // Typically the bow’s owner

        if (InstigatorActor)
        {
            if (UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor))
            {
                if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
                {
                    // Build context
                    FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
                    Context.AddSourceObject(this);
                    Context.AddHitResult(Hit);

                    // Create outgoing spec
                    FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, 1.f, Context);
                    if (SpecHandle.IsValid())
                    {
                        // Use SetByCaller with BaseDamage stored on the arrow
                        static FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Event.Ranged"));
                        SpecHandle.Data->SetSetByCallerMagnitude(DamageTag, BaseDamage);

                        // Apply to target
                        SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);

                        UE_LOG(LogTemp, Warning, TEXT("ArrowProjectile: Applied %f ranged damage to %s"), BaseDamage, *OtherActor->GetName());
                    }
                }
            }
        }
    }

    if (bDestroyOnHit)
    {
        Destroy();
    }
}
