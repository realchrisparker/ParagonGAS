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
 * Date: 7-23-2025
 * =============================================================================
 * UPGAS_StateTreeTaskBlueprintBase
 * A base class for creating blueprint-friendly StateTree tasks for Paragon GAS Sample.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "PGAS_StateTreeTaskBlueprintBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Category = "Enemy AI", meta = (DisplayName = "State Tree Base Task"))
class PARAGONGAS_API UPGAS_StateTreeTaskBlueprintBase : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	/*
	 * Functions
	*/
	
	/** Called when the state is entered. Override in BP/C++. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "StateTree|Task")
	void OnEnterState(const FStateTreeTransitionResult& Transition);
	virtual void OnEnterState_Implementation(const FStateTreeTransitionResult& Transition);

	/** Called when the state is exited. Override in BP/C++. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "StateTree|Task")
	void OnExitState(const FStateTreeTransitionResult& Transition);
	virtual void OnExitState_Implementation(const FStateTreeTransitionResult& Transition);

	/** Called every tick if enabled. Override in BP/C++. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "StateTree|Task")
	void OnTick(float DeltaTime);
	virtual void OnTick_Implementation(float DeltaTime);

	/** Called when the state is completed. Override in BP/C++. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "StateTree|Task")
	void OnStateCompleted(EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates);
	virtual void OnStateCompleted_Implementation(EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates);

	/*
	 * Properties
	*/

	/** The enemy character this task is associated with. */
	UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
	TObjectPtr<APGAS_EnemyCharacter> Actor;

	/** The AI controller managing the enemy character. */
	UPROPERTY(BlueprintReadOnly, Category = Context, meta = (Context))
	TObjectPtr<APGAS_EnemyAIController> AIController;
};
