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
 * Date: 7-22-2025
 * =============================================================================
 * Custom StateTree Component for enhanced debugging and control.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeAIComponent.h"
#include "StateTreeInstanceData.h"
#include "GameplayTagContainer.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "StateTreeEvents.h"
#include "StateTreeExecutionContext.h"
#include "PGAS_StateTreeAIComponent.generated.h"

UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent, DisplayName = "PGAS State Tree AI Component"))
class PARAGONGAS_API UPGAS_StateTreeAIComponent : public UStateTreeAIComponent
{
    GENERATED_BODY()

public:
    /*
     * Functions
    */

    // Constructor
    UPGAS_StateTreeAIComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Called when the game starts or when spawned */
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Called when the component is initialized */
    virtual void InitializeComponent() override;

    /** Starts the logic for the State Tree */
    virtual void StartLogic() override;

    /** Stops the logic for the State Tree */
    virtual void StopLogic(const FString& Reason) override;

    /** Sets the State Tree for this component */
    virtual void ValidateStateTreeReference() override;

    /**
     * Sets the State Tree for this component.
     * @param StateTree The State Tree to set.
    */
    void SetStateTree(UStateTree* StateTree);

    /**
     * Sets the State Tree reference for this component and starts the logic.
     * @param StateTreeReference The State Tree reference to set.
    */
    UFUNCTION(BlueprintCallable, Category = "StateTree")
    void StartStateTree(UStateTree* StateTree);

    UFUNCTION(BlueprintCallable, Category = "StateTree")
    void SendEvent(const FGameplayTag& EventTag);

    // /**
    //  * Gets the value of the 'DefaultMotivation' parameter from the State Tree instance.
    //  * @return A const reference to the gameplay tag value. Returns an empty tag if not found.
    // */
    // UFUNCTION(BlueprintCallable, Category = "StateTree")
    // const FGameplayTag& GetDefaultMotivationParameter() const
    // {
    //     // The InstanceData member is accessible because it's a protected member of the parent class UStateTreeComponent.
    //     // We get a const view of the struct that holds all the global parameters.
    //     const FConstStructView Parameters = InstanceData.GetStorage().GetGlobalParameters();

    //     if (Parameters.IsValid())
    //     {
    //         // The FName must exactly match the parameter name in the State Tree asset.
    //         const FName ParamName(TEXT("DefaultMotivation"));

    //         // Find the FProperty for our tag within the parameters struct.
    //         if (const FProperty* Property = Parameters.GetScriptStruct()->FindPropertyByName(ParamName))
    //         {
    //             // Get a const pointer to the actual data.
    //             const FGameplayTag* TagPtr = Property->ContainerPtrToValuePtr<const FGameplayTag>(Parameters.GetMemory());
    //             return *TagPtr; // Return a const reference to the found tag.
    //         }
    //     }

    //     // If the parameter is not found for any reason, log a warning for debugging.
    //     UE_LOG(LogTemp, Warning, TEXT("GetDefaultMotivationParameter: Could not find 'Default Motivation' parameter. Returning an empty tag."));

    //     // Return a safe, static empty tag. This prevents returning a dangling reference.
    //     return FGameplayTag::EmptyTag;
    // }

    // /**
    //  * Gets the value of the 'Motivation' parameter from the State Tree instance.
    //  * @return A const reference to the gameplay tag value. Returns an empty tag if not found.
    // */
    // UFUNCTION(BlueprintCallable, Category = "StateTree")
    // const FGameplayTag& GetMotivationParameter() const
    // {
    //     // The InstanceData member is accessible because it's a protected member of the parent class UStateTreeComponent.
    //     // We get a const view of the struct that holds all the global parameters.
    //     const FConstStructView Parameters = InstanceData.GetStorage().GetGlobalParameters();

    //     if (Parameters.IsValid())
    //     {
    //         // The FName must exactly match the parameter name in the State Tree asset.
    //         const FName ParamName(TEXT("Motivation"));

    //         // Find the FProperty for our tag within the parameters struct.
    //         if (const FProperty* Property = Parameters.GetScriptStruct()->FindPropertyByName(ParamName))
    //         {
    //             // Get a const pointer to the actual data.
    //             const FGameplayTag* TagPtr = Property->ContainerPtrToValuePtr<const FGameplayTag>(Parameters.GetMemory());
    //             if (TagPtr)
    //             {
    //                 // Return a const reference to the found tag.
    //                 return *TagPtr;
    //             }
    //         }
    //     }

    //     // If the parameter is not found for any reason, log a warning for debugging.
    //     UE_LOG(LogTemp, Warning, TEXT("GetMotivationParameter: Could not find 'Motivation' parameter. Returning an empty tag."));

    //     // Return a safe, static empty tag. This prevents returning a dangling reference.
    //     return FGameplayTag::EmptyTag;
    // }

    // /**
    //  * Sets the value of the 'Motivation' parameter in the State Tree instance.
    //  * @param NewMotivation The new gameplay tag to set.
    // */
    // UFUNCTION(BlueprintCallable, Category = "StateTree")
    // void SetMotivationParameter(const FGameplayTag& NewMotivation)
    // {
    //     // The InstanceData member is protected in UStateTreeComponent, so we can access it here.
    //     // We get a mutable view of the struct that holds all the parameters.
    //     FStructView Parameters = InstanceData.GetMutableStorage().GetMutableGlobalParameters();

    //     if (Parameters.IsValid())
    //     {
    //         // The FName must exactly match the parameter name in the State Tree asset.
    //         const FName MotivationPropName = FName("Motivation");

    //         // Find the FProperty for our tag within the parameters struct.
    //         if (const FProperty* Property = Parameters.GetScriptStruct()->FindPropertyByName(MotivationPropName))
    //         {
    //             // Get a pointer to the actual data and update it.
    //             FGameplayTag* MotivationTagPtr = Property->ContainerPtrToValuePtr<FGameplayTag>(Parameters.GetMemory());
    //             if (MotivationTagPtr)
    //             {
    //                 *MotivationTagPtr = NewMotivation;
    //                 const FGameplayTag& updatedTag = GetMotivationParameter(); // Refresh the tag reference
    //                 UE_LOG(LogTemp, Warning, TEXT("Motivation set to: %s"), *updatedTag.GetTagName().ToString());
    //                 return; // Successfully set the parameter
    //             }
    //         }
    //     }

    //     UE_LOG(LogTemp, Warning, TEXT("SetMotivationParameter: Could not find or set 'Motivation' parameter!"));
    // }

    // /**
    //  * Gets the value of the 'Acquired Target Actor' parameter from the State Tree instance.
    //  * @return The actor pointer if found, otherwise nullptr.
    // */
    // UFUNCTION(BlueprintCallable, Category = "StateTree")
    // AActor* GetAcquiredTargetActorParameter()
    // {
    //     const FConstStructView Parameters = InstanceData.GetStorage().GetGlobalParameters();

    //     if (Parameters.IsValid())
    //     {
    //         const FName ParamName(TEXT("Acquired_Target_Actor"));

    //         if (const FProperty* Property = Parameters.GetScriptStruct()->FindPropertyByName(ParamName))
    //         {
    //             // Use FObjectProperty for both raw and TObjectPtr<AActor>
    //             if (const FObjectProperty* ObjProp = CastField<const FObjectProperty>(Property))
    //             {
    //                 return Cast<AActor>(ObjProp->GetObjectPropertyValue(Parameters.GetMemory()));
    //             }
    //         }
    //     }

    //     UE_LOG(LogTemp, Warning, TEXT("GetAcquiredTargetActorParameter: Could not find or access 'Acquired Target Actor' parameter. Returning nullptr."));
    //     return nullptr;
    // }

    // /**
    //  * Sets the acquired target actor for the AI component.
    //  * @param NewTargetActor The new target actor to set.
    // */
    // UFUNCTION(BlueprintCallable, Category = "StateTree")
    // void SetAcquiredTargetActorParameter(AActor* NewTargetActor)
    // {
    //     // Get a mutable view of the struct that holds all the global parameters.
    //     FStructView Parameters = InstanceData.GetMutableStorage().GetMutableGlobalParameters();

    //     if (Parameters.IsValid())
    //     {
    //         const FName ParamName(TEXT("Acquired_Target_Actor"));

    //         // Find the FProperty for our actor pointer within the parameters struct.
    //         if (const FProperty* Property = Parameters.GetScriptStruct()->FindPropertyByName(ParamName))
    //         {
    //             // Use FObjectProperty for both TObjectPtr<AActor> and AActor* fields.
    //             if (FObjectProperty* ObjProp = CastField<FObjectProperty>(const_cast<FProperty*>(Property)))
    //             {
    //                 ObjProp->SetObjectPropertyValue(Parameters.GetMemory(), NewTargetActor);
    //                 // UE_LOG(LogTemp, Warning, TEXT("SetAcquiredTargetActorParameter: StateTree parameter 'Acquired Target Actor' set to: %s"),
    //                 //     *GetNameSafe(NewTargetActor));
    //                 return;
    //             }
    //         }
    //     }

    //     UE_LOG(LogTemp, Warning, TEXT("SetAcquiredTargetActorParameter: Could not find or set 'Acquired Target Actor' parameter!"));
    // }

    // /**
    //  * Gets the last known target location parameter from the State Tree instance.
    //  * @return The last known target location as a FVector.
    // */
    // UFUNCTION(BlueprintCallable, Category = "StateTree")
    // FVector GetPointOfInterestParameter()
    // {
    //     FStructView Parameters = InstanceData.GetMutableStorage().GetMutableGlobalParameters();

    //     if (Parameters.IsValid())
    //     {
    //         const FName ParamName(TEXT("Point_of_Interest"));

    //         if (const FProperty* Property = Parameters.GetScriptStruct()->FindPropertyByName(ParamName))
    //         {
    //             // Use FStructProperty for struct types like FVector.
    //             if (const FStructProperty* StructProp = CastField<const FStructProperty>(Property))
    //             {
    //                 if (StructProp->Struct == TBaseStructure<FVector>::Get())
    //                 {
    //                     const FVector* VectorPtr = StructProp->ContainerPtrToValuePtr<FVector>(Parameters.GetMemory());
    //                     if (VectorPtr)
    //                     {
    //                         return *VectorPtr;
    //                     }
    //                 }
    //             }
    //         }
    //     }

    //     UE_LOG(LogTemp, Warning, TEXT("GetPointOfInterestParameter: Could not find or access 'Point Of Interest'. Returning zero vector."));
    //     return FVector::ZeroVector;
    // }

    // /**
    //  * Sets the last known target location.
    //  * @return The last known target location.
    // */
    // UFUNCTION(BlueprintCallable, Category = "StateTree")
    // void SetPointOfInterestParameter(const FVector& NewLocation)
    // {
    //     FStructView Parameters = InstanceData.GetMutableStorage().GetMutableGlobalParameters();

    //     if (Parameters.IsValid())
    //     {
    //         const FName ParamName(TEXT("Point_of_Interest"));

    //         if (FProperty* Property = Parameters.GetScriptStruct()->FindPropertyByName(ParamName))
    //         {
    //             if (FStructProperty* StructProp = CastField<FStructProperty>(Property))
    //             {
    //                 if (StructProp->Struct == TBaseStructure<FVector>::Get())
    //                 {
    //                     FVector* VectorPtr = StructProp->ContainerPtrToValuePtr<FVector>(Parameters.GetMemory());
    //                     if (VectorPtr)
    //                     {
    //                         *VectorPtr = NewLocation;
    //                         // UE_LOG(LogTemp, Warning, TEXT("SetPointOfInterestParameter: 'Point Of Interest' set to: %s"), *NewLocation.ToString());
    //                         return;
    //                     }
    //                 }
    //             }
    //         }
    //     }

    //     UE_LOG(LogTemp, Warning, TEXT("SetPointOfInterestParameter: Could not find or set 'Point Of Interest'!"));
    // }

    // /**
    //  * Gets the visibility state of the acquired target.
    //  * @return True if the acquired target is visible, false otherwise.
    // */
    // UFUNCTION(BlueprintCallable, Category = "StateTree")
    // bool GetIsAcquiredTargetVisibleParameter() const
    // {
    //     const FConstStructView Parameters = InstanceData.GetStorage().GetGlobalParameters();

    //     if (Parameters.IsValid())
    //     {
    //         const FName ParamName(TEXT("Is_Acquired_Target_Visible"));

    //         if (const FProperty* Property = Parameters.GetScriptStruct()->FindPropertyByName(ParamName))
    //         {
    //             if (const FBoolProperty* BoolProp = CastField<const FBoolProperty>(Property))
    //             {
    //                 const bool* BoolPtr = BoolProp->ContainerPtrToValuePtr<bool>(Parameters.GetMemory());
    //                 if (BoolPtr)
    //                 {
    //                     return *BoolPtr;
    //                 }
    //             }
    //         }
    //     }

    //     UE_LOG(LogTemp, Warning, TEXT("GetIsAcquiredTargetVisibleParameter: Could not find or access 'Is Acquired Target Visible'. Returning false."));
    //     return false;
    // }

    // /**
    //  * Sets the visibility state of the acquired target.
    //  * @param bNewVisible The new visibility state to set.
    // */
    // UFUNCTION(BlueprintCallable, Category = "StateTree")
    // void SetIsAcquiredTargetVisibleParameter(bool bNewVisible)
    // {
    //     FStructView Parameters = InstanceData.GetMutableStorage().GetMutableGlobalParameters();

    //     if (Parameters.IsValid())
    //     {
    //         const FName ParamName(TEXT("Is_Acquired_Target_Visible"));

    //         if (FProperty* Property = Parameters.GetScriptStruct()->FindPropertyByName(ParamName))
    //         {
    //             if (FBoolProperty* BoolProp = CastField<FBoolProperty>(Property))
    //             {
    //                 bool* BoolPtr = BoolProp->ContainerPtrToValuePtr<bool>(Parameters.GetMemory());
    //                 if (BoolPtr)
    //                 {
    //                     *BoolPtr = bNewVisible;
    //                     // UE_LOG(LogTemp, Warning, TEXT("SetIsAcquiredTargetVisibleParameter: 'Is Acquired Target Visible' set to: %s"), bNewVisible ? TEXT("true") : TEXT("false"));
    //                     return;
    //                 }
    //             }
    //         }
    //     }

    //     UE_LOG(LogTemp, Warning, TEXT("SetIsAcquiredTargetVisibleParameter: Could not find or set 'Is Acquired Target Visible'!"));
    // }

private:
    // void ShowEditorNotification(const FText& Message, SNotificationItem::ECompletionState State);
};
