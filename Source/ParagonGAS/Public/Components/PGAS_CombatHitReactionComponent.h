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
#include <Structs/PGAS_HitReaction.h>
#include <Data/PGAS_HitReactionDataObject.h>
#include "PGAS_CombatHitReactionComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (PGAS),
	meta = (BlueprintSpawnableComponent,
		DisplayName = "Hit Reaction Component")
)
class PARAGONGAS_API UPGAS_CombatHitReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/*
	* Functions
	*/
	
	// Sets default values for this component's properties
	UPGAS_CombatHitReactionComponent();

	/**
	 * Performs a hit reaction based on the specified hit direction.
	 * @param HitDirection The direction of the hit (e.g., "Hit.Back", "Hit.Forward", etc.).
	 * @param PlayRate The play rate for the hit reaction montage.
	*/
	UFUNCTION(BlueprintCallable, Category = "Hit Reaction")
	void PerformHitReaction(EPGAS_HitDirection Direction, EPGAS_HitSeverity Severity);

	/*
	* Properties
	*/

	/** All available hit reactions (configured in editor) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction", meta = (DisplayName = "Hit Reactions"))
	TArray<FPGAS_HitReaction> HitReactions;

protected:

	/*
	* Functions
	*/

	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/*
	* Functions
	*/
	
	/** Helper: Plays a montage on the owning actor's mesh if valid */
	void PlayMontageInternal(UAnimMontage* Montage, float InPlayRate = 1.0f);

	/**
	 * Finds the best matching reaction based on direction and severity.
	 * @param Direction The hit direction.
	 * @param Severity The hit severity.
	 * @return The best matching hit reaction, or nullptr if none found.
	 */
	const FPGAS_HitReaction* FindReaction(EPGAS_HitDirection Direction, EPGAS_HitSeverity Severity) const;

	/*
	* Properties
	*/

	/** Caches the owner as a character */
	UPROPERTY()
	TObjectPtr<class APGAS_CharacterBase> CachedOwnerCharacter;
};
