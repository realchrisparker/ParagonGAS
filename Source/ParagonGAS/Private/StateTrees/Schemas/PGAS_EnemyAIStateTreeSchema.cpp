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

#include <StateTrees/Schemas/PGAS_EnemyAIStateTreeSchema.h>
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Controllers/AI/PGAS_EnemyAIController.h>
#include "StateTreeExecutionContext.h"
#include "StateTreeExecutionTypes.h"
#include "Misc/Guid.h"


TArray<FStateTreeExternalDataDesc> UPGAS_EnemyAIStateTreeSchema::ContextDataDescs;

UPGAS_EnemyAIStateTreeSchema::UPGAS_EnemyAIStateTreeSchema()
{
    // Schema settings (seen in StateTree Editor)
    AIControllerClass = APGAS_EnemyAIController::StaticClass(); // Set the AIController class for this schema
    ContextActorClass = APGAS_EnemyCharacter::StaticClass(); // Set the context actor class for this schema

    // Initialize context (AIController and Actor seen in StateTree Editor)
    // This is done once to avoid repeated allocations and ensure consistency across executions.
    if (ContextDataDescs.IsEmpty())
    {
        // Built-in Context Actor
        ContextDataDescs.Add(FStateTreeExternalDataDesc(
            FName(TEXT("Actor")),
            APGAS_EnemyCharacter::StaticClass(),
            FGuid::NewGuid() // For release, hardcode this for persistence!
        ));

        // Built-in AIController
        ContextDataDescs.Add(FStateTreeExternalDataDesc(
            FName(TEXT("AIController")),
            APGAS_EnemyAIController::StaticClass(),
            FGuid::NewGuid()
        ));

    }
}

TConstArrayView<FStateTreeExternalDataDesc> UPGAS_EnemyAIStateTreeSchema::GetContextDataDescs() const
{
    return ContextDataDescs;
}

/*
 * This function is called to validate if a struct is allowed in the StateTree context.
 * It can be used to restrict which structs can be used in the StateTree.
 * @param InScriptStruct The struct to check.
 * @return true if the struct is allowed, false otherwise.
 * @notes This allows the editor to see our custom structs like FPGAS_CompareGameplayTagCondition, etc. By default the base class returns false so our custom structs are not visible.
*/
bool UPGAS_EnemyAIStateTreeSchema::IsStructAllowed(const UScriptStruct* InScriptStruct) const
{
    // if (Super::IsStructAllowed(InScriptStruct))
    // {
    //     return true;
    // }

    // // Add your custom condition structs here
    // if (InScriptStruct == FPGAS_CompareGameplayTagCondition::StaticStruct())
    // {
    //     return true;
    // }

    return true; // Allow all structs for now
}