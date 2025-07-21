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
 * Date: 7-13-2025
 * =============================================================================
 * Custom AI Controller for Enemy Characters (StateTree-ready)
 */

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StateTreeReference.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Touch.h"
#include "Perception/AISenseConfig_Prediction.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include "PGAS_EnemyAIController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent, DisplayName="Enemy AI Controller"))
class PARAGONGAS_API APGAS_EnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	/*
	 * Functions
	*/

	// Constructor
	APGAS_EnemyAIController();

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called when the controller possesses a pawn */
	virtual void OnPossess(APawn* InPawn) override;

	/** Called when the controller is unpossessed */
	virtual void OnUnPossess() override;

protected:
	/*
	 * Properties
	*/

	// Sight configuration object
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	// Hearing config
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;	

	// Damage config
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;

	/*
	 * Functions
	*/
	
	// Perception updated callback
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	/*
	 * Properties
	*/

	/** Enemy character this controller is managing */
	TObjectPtr<APGAS_EnemyCharacter> OwningCharacter = nullptr;
};
