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
 * PGAS_AnimInstanceBase.h
 * Base class for all Animation Instances in ParagonGAS.
 * This class serves as a foundation for custom animation instances, providing a common interface and functionality.
*/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PGAS_AnimInstanceBase.generated.h"

/**
 * Base class for all Animation Instances in ParagonGAS
 */
UCLASS(meta=(DisplayName="PGAS Anim Instance Base", ShortTooltip="Base class for all Animation Instances in ParagonGAS"))
class PARAGONGAS_API UPGAS_AnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	/*
	 * Functions
	*/

	// Constructor
	UPGAS_AnimInstanceBase(const FObjectInitializer& ObjectInitializer);

	// Returns the movement component of the character, thread-safe
	// This function is thread-safe and can be called from any thread.
	UFUNCTION(BlueprintPure, Category = "Animation", meta = (BlueprintThreadSafe))
	const UCharacterMovementComponent* GetMovementComponent() const { return CachedMovementComponent.Get(); }

	// Returns the owning pawn of the animation instance, thread-safe
	// This function is thread-safe and can be called from any thread.
	UFUNCTION(BlueprintPure, Category = "Animation", meta = (BlueprintThreadSafe))
	APawn* GetOwningPawn() { return OwningPawn.Get(); }

	/*
	 * Properties
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	float Roll;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	float Yaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	float YawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	FRotator RotationLastTick;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool FullBody;

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

private:
	/*
	 * Properties
	*/

	// Cached movement component pointer (game thread only)
	TObjectPtr<const UCharacterMovementComponent> CachedMovementComponent;

	// Cached movement component pointer (game thread only)
	TObjectPtr<APawn> OwningPawn;	
};
