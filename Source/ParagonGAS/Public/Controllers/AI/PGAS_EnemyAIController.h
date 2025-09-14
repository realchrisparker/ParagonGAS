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
#include "GameplayTagContainer.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Touch.h"
#include "Perception/AISenseConfig_Prediction.h"
#include "GenericTeamAgentInterface.h"
#include <Characters/Enemy/PGAS_EnemyCharacter.h>
#include <Characters/Player/PGAS_PlayerCharacter.h>
#include <Components/PGAS_StateTreeAIComponent.h">
#include <Enums/PGAS_StimulusSenseType.h>
#include <Enums/PGAS_StateTreeEvent.h>
#include "PGAS_EnemyAIController.generated.h"


 // Delegate for perception updates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPGASStimulusEventSignature, AActor*, Actor, const FAIStimulus&, Stimulus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPGASStimulusForgottenSignature, AActor*, Actor);


/**
 * Custom AI Controller for Enemy Characters
 * This controller manages the AI behavior for enemy characters using State Trees.
*/
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent, DisplayName = "Enemy AI Controller"))
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

	// Returns the State Tree AI Component for this character
	// This component handles the AI behavior for the enemy character.
	UFUNCTION(BlueprintCallable, Category = "Player|AI", meta = (DisplayName = "Get State Tree AI Component"))
	UPGAS_StateTreeAIComponent* GetStateTreeAIComponent() const { return StateTreeAIComponent; };

	UFUNCTION(BlueprintCallable, Category = "Player|AI", meta = (DisplayName = "Get Current Stimulus Sense Type"))
	EPGAS_StimulusSenseType GetCurrentStimulusSenseType() const { return CurrentStimulusSenseType; };

	/**
	 * Forget a specific actor from perception.
	 * @param ActorToForget The actor to forget.
	*/
	UFUNCTION(BlueprintCallable, Category = "Player|AI")
	void ForgetPerceptionActor(AActor* ActorToForget);

	/**
	 * Forget multiple actors from perception.
	 * @param ActorsToForget The array of actors to forget.
	*/
	UFUNCTION(BlueprintCallable, Category = "Player|AI")
	void ForgetPerceptionActors(const TArray<AActor*>& ActorsToForget);

	/**
	 * Returns all actors currently sensed by the Damage sense.
	 * This function retrieves all actors that have been sensed by the Damage sense.
	 * @return An array of actors currently sensed by the Damage sense.
	*/
	UFUNCTION(BlueprintCallable, Category = "Player|AI")
    TArray<AActor*> GetAllDamageSensedActors() const;

	/**
	 * Returns all actors currently sensed by the Hearing sense.
	 * This function retrieves all actors that have been sensed by the Hearing sense.
	 * @return An array of actors currently sensed by the Hearing sense.
	*/
	UFUNCTION(BlueprintCallable, Category = "Player|AI")
    TArray<AActor*> GetAllHeardActors() const;

	/**
	 * Returns all actors currently sensed by the Sight sense.
	 * This function retrieves all actors that have been sensed by the Sight sense.
	 * @return An array of actors currently sensed by the Sight sense.
	*/
	UFUNCTION(BlueprintCallable, Category = "Player|AI")
	TArray<AActor*> GetAllSeenActors() const;

	/**
	 * Report a damage event to the AI perception system.
	 * This function is used to report damage events to the AI perception system.
	 * @param DamagedActor The actor that was damaged.
	 * @param InstigatorActor The actor that caused the damage.
	 * @param DamageAmount The amount of damage inflicted.
	*/
	UFUNCTION(BlueprintCallable, Category = "AI|Perception")
	void ReportDamageEvent(AActor* DamagedActor, AActor* InstigatorActor, float DamageAmount);

	/**
	 * Report a noise event to the AI perception system.
	 * This function is used to report noise events to the AI perception system.
	 * @param NoiseInstigator The actor that made the noise.
	 * @param NoiseLocation The location where the noise was made.
	 * @param Loudness The loudness of the noise (default is 1.0).
	 * @param MaxRange The maximum range of the noise (default is 1200.0).
	*/
	UFUNCTION(BlueprintCallable, Category = "AI|Perception")
	void ReportNoiseEvent(AActor* NoiseInstigator, FVector NoiseLocation, float Loudness = 1.f, float MaxRange = 1200.f);

	/*
	 * Properties
	*/

	UPROPERTY(BlueprintAssignable, Category = "Player|AI|Events", meta = (DisplayName = "On Damage Stimulus Detected"))
	FPGASStimulusEventSignature OnDamageStimulusDetected;

	/** Broadcast delegates */
	UPROPERTY(BlueprintAssignable, Category = "Player|AI|Events", meta = (DisplayName = "On Sight Stimulus Detected"))
	FPGASStimulusEventSignature OnSightStimulusDetected;

	UPROPERTY(BlueprintAssignable, Category = "Player|AI|Events", meta = (DisplayName = "On Sight Stimulus Forgotten"))
	FPGASStimulusForgottenSignature OnSightStimulusForgotten;

	UPROPERTY(BlueprintAssignable, Category = "Player|AI|Events", meta = (DisplayName = "On Hearing Stimulus Detected"))
	FPGASStimulusEventSignature OnHearingStimulusDetected;

	UPROPERTY(BlueprintAssignable, Category = "Player|AI|Events", meta = (DisplayName = "On Hearing Stimulus Forgotten"))
	FPGASStimulusForgottenSignature OnHearingStimulusForgotten;

	/**
	 * The current acquired target from perception events 
	 * Placed here for easy access in Blueprints and can be shared in State Trees.
	 * This is the actor that the AI is currently focused on due to perception.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Output, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> AcquiredTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Output, meta = (AllowPrivateAccess = "true"))
	EPGAS_StateTreeEvent CurrentStateTreeState;

protected:

	/*
	 * Functions
	*/

	// Perception updated callback
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnTargetPerceptionForgotten(AActor* Actor);

private:
	/*
	 * Properties
	*/

	// State tree AI component for managing enemy AI behavior
	// This component handles the AI behavior for the enemy character.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPGAS_StateTreeAIComponent> StateTreeAIComponent;

	/** Enemy character this controller is managing */
	TObjectPtr<APGAS_EnemyCharacter> OwningCharacter = nullptr;

	// Sight configuration object
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	// Hearing config
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	// Damage config
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;

	// Prediction config
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Prediction> PredictionConfig;

	UPROPERTY()
	EPGAS_StimulusSenseType CurrentStimulusSenseType = EPGAS_StimulusSenseType::Unknown;

	/*
	 * Functions
	*/

	// Override team attitude to filter perception targets
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
