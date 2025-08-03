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
 * Date: 7-30-2025
 * =============================================================================
 * PGAS_UpdateMovementSpeed.h
 * State Tree task that changes an APGAS_CharacterBase’s MaxWalkSpeed.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_UpdateMovementSpeed.generated.h"


/**
 * Updates a character’s movement speed when the task enters the state.
 */
UCLASS(Blueprintable, BlueprintType, Category = "PGAS", meta = (DisplayName = "Update Movement Speed"))
class PARAGONGAS_API UPGAS_UpdateMovementSpeed : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	/*
	 * Properties
	*/

	/** The enemy character this task is associated with. */
	UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
	TObjectPtr<APGAS_EnemyCharacter> Actor;

	/** The AI controller managing the enemy character. */
	UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
	TObjectPtr<APGAS_EnemyAIController> AIController;

	/** Desired MaxWalkSpeed to apply while this task is active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0"))
	float TargetMaxWalkSpeed = 600.0f;	

	/*
	 * Functions
	*/

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif

protected:
	/*
	 * Functions
	*/

	/**
	 * This function sets the character's MaxWalkSpeed to the TargetMaxWalkSpeed when entering the state.
	 * @param Context The execution context for the state tree.
	 * @param Transition The transition result containing information about the state change.
	*/
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
};
