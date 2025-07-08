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
 * Date: 7-7-2025
 * =============================================================================
 * PGAS_PlayerAnimInstanceBase.h
 * Base class for all Player Animation Instances in ParagonGAS.
*/

#pragma once

#include "CoreMinimal.h"
#include "Animations/Base/PGAS_AnimInstanceBase.h"
#include "Characters/Player/PGAS_PlayerCharacter.h"
#include "PGAS_PlayerAnimInstanceBase.generated.h"

/**
 * Player character class  Animation Instances
 */
UCLASS(meta = (DisplayName = "PGAS Player Anim Instance Base", ShortTooltip = "Base class for all Player Animation Instances in ParagonGAS"))
class PARAGONGAS_API UPGAS_PlayerAnimInstanceBase : public UPGAS_AnimInstanceBase
{
	GENERATED_BODY()

public:
	/*
	 * Functions
	*/

	// Constructor
	UPGAS_PlayerAnimInstanceBase(const FObjectInitializer& ObjectInitializer);

	// Returns the owning character of the animation instance, thread-safe
	// This function is thread-safe and can be called from any thread.
	UFUNCTION(BlueprintPure, Category = "Animation", meta = (BlueprintThreadSafe))
	APGAS_PlayerCharacter* GetOwningCharacter() { return OwningCharacter.Get(); }

	/*
	 * Properties
	*/

protected:
	/*
	 * Functions
	*/

	// Called before destroying the object.
	virtual void BeginDestroy() override;

	/** Called when the animation instance is initialized */
	virtual void NativeInitializeAnimation() override;

	// Called every frame
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Native thread-safe update for any work not relying on game thread
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	/*
	 * Properties
	*/

	// Cached movement component pointer (game thread only)
	UPROPERTY()
	TObjectPtr<APGAS_PlayerCharacter> OwningCharacter;

private:
};
