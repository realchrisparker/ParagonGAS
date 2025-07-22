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
 * Date: 7-12-2025
 * =============================================================================
 * PGAS_HitReactionComponent
 * Handles hit reaction logic for actors (flinch, stagger, hit animations, VFX, etc.).
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "GameplayTagContainer.h"
#include "PGAS_HitReactionComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, DisplayName = "Hit Reaction Component"))
class PARAGONGAS_API UPGAS_HitReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/*
	* Functions
	*/
	
	// Sets default values for this component's properties
	UPGAS_HitReactionComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Performs a hit reaction based on the specified hit direction.
	 * @param HitDirection The direction of the hit (e.g., "Hit.Back", "Hit.Forward", etc.).
	 * @param PlayRate The play rate for the hit reaction montage.
	*/
	UFUNCTION(BlueprintCallable, Category = "Hit Reaction")
	void PerformHitReaction(const FGameplayTag& HitDirection, float PlayRate = 1.0f);

	/*
	* Properties
	*/

	// Default hit reaction montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction|Animation")
	TObjectPtr<UAnimMontage> DefaultHitMontage;

	// Hit reaction montages for being hit from back.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction|Animation")
	TObjectPtr<UAnimMontage> BackHitMontage;

	// Hit reaction montages for being hit from front.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction|Animation")
	TObjectPtr<UAnimMontage> ForwardHitMontage;

	// Hit reaction montages for being hit from left.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction|Animation")
	TObjectPtr<UAnimMontage> LeftHitMontage;

	// Hit reaction montages for being hit from right.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction|Animation")
	TObjectPtr<UAnimMontage> RightHitMontage;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/*
	* Functions
	*/
	
	/** Helper: Plays a montage on the owning actor's mesh if valid */
	virtual void PlayMontageInternal(UAnimMontage* Montage, float InPlayRate = 1.0f);

	/*
	* Properties
	*/

	/** Caches the owner as a character */
	UPROPERTY()
	TObjectPtr<class APGAS_CharacterBase> CachedOwnerCharacter;
};
