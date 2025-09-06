/*
 * ========================================================================
 * Copyright © 2025 God's Studio
 * All Rights Reserved.
 *
 * Free for all to use, copy, and distribute. I hope you learn from this as I learned creating it.
 * =============================================================================
 *
 * Project: Paragon GAS Sample
 * Author: Christopher D. Parker
 * Date: 9-4-2025
 * =============================================================================
 * ComboGraph.h
 * Defines the core graph object used by the Combat Combo Graph Editor.
 */

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include <ComboGraphEditor/Schema/ComboGraphSchema.h>
#include "ComboGraph.generated.h"

/**
  * The graph object for Combat Combo Graphs.
  * This is the container that holds nodes (Sequence/Combo).
  */
UCLASS()
class PARAGONGASEDITOR_API UComboGraph : public UEdGraph
{
    GENERATED_BODY()

public:
    /*
    * Functions
    */

    // Constructor
    UComboGraph()
    {
        // Assign the schema for this graph
        this->Schema = UComboGraphSchema::StaticClass();
    }

    /** Simple accessor for debugging/logging */
    UFUNCTION(BlueprintCallable, Category = "ComboGraph", meta = (DisplayName = "Get Graph Name"))
    FString GetGraphName() const
    {
        return GetName();
    }

    /**
     * Accessor for the graph's schema with proper typecasting.
     * Use this to access schema-specific functionality without repeated casting.
     * @return The graph's schema cast to UComboGraphSchema.
     */
    UComboGraphSchema* GetGraphSchema() const
    {
        return const_cast<UComboGraphSchema*>(Cast<UComboGraphSchema>(this->GetSchema()));
    }
};
