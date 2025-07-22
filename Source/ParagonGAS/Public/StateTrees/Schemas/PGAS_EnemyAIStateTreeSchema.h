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
 * Date: 7-26-2025
 * =============================================================================
 * PGAS_StateTreeSchema
 * Custom StateTree schema for validating context and data for Paragon GAS AI logic.
 */

//// Example of a custom StateTree schema for Paragon GAS AI. This sample uses the built-in StateTreeAIComponentSchema, but this is how a custom one is built.
//// Left in for future reference and learning purposes.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeAIComponentSchema.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Components/PGAS_StateTreeAIComponent.h>
#include "PGAS_EnemyAIStateTreeSchema.generated.h"

class APGAS_EnemyCharacter;
class UPGAS_StateTreeAIComponent;

/**
 * Custom StateTree schema for Paragon GAS AI.
 * This schema provides context validation and data retrieval for AI logic.
 */
UCLASS(Blueprintable, BlueprintType, meta = (DisplayName = "PGAS Enemy AI State Tree Schema"))
class PARAGONGAS_API UPGAS_EnemyAIStateTreeSchema : public UStateTreeAIComponentSchema
{
	GENERATED_BODY()

public:
	/*
	 * Functions
	*/
	
	// Constructor
	UPGAS_EnemyAIStateTreeSchema();

	// Returns the context data descriptions for the schema.
	virtual TConstArrayView<FStateTreeExternalDataDesc> GetContextDataDescs() const override;

	/*
	 * Returns the context description for the given context.
	 * This is used to validate the context data for the StateTree.
	*/
	virtual bool IsStructAllowed(const UScriptStruct* InScriptStruct) const override;

private:
	/*
	 * Properties
	*/
	
	/** Cache the context description for performance and reuse */
	static TArray<FStateTreeExternalDataDesc> ContextDataDescs;
};
