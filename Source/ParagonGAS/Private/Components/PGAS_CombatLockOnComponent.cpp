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
 * Date: 8-30-2025
 * =============================================================================
 * PGAS_LockOnComponent.cpp
 * Implementation for lock-on targeting. Designed to be gameplay-agnostic and UI friendly.
 */

#include <Components/PGAS_CombatLockOnComponent.h>
#include <Characters/Base/PGAS_CharacterBase.h>
#include <UserWidgets/PGAS_Reticle.h>
#include <Interfaces/IAnimation.h>
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"


// Constructor
UPGAS_CombatLockOnComponent::UPGAS_CombatLockOnComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;

    // Default to your unified character base so *all* characters are valid lock targets.
    TargetClassFilter = APGAS_CharacterBase::StaticClass();
}

void UPGAS_CombatLockOnComponent::BeginPlay()
{
    Super::BeginPlay();

    // Cache movement comp if owner is a Character (for rotation policy flips).
    if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
    {
        CachedMoveComp = Char->GetCharacterMovement();
        if (CachedMoveComp)
        {
            bSavedOrientRotationToMovement = CachedMoveComp->bOrientRotationToMovement;
            bSavedUseControllerDesiredRotation = CachedMoveComp->bUseControllerDesiredRotation;
        }
    }
}

void UPGAS_CombatLockOnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    InternalUnlock(/*bBroadcast=*/false);
    Super::EndPlay(EndPlayReason);
}

void UPGAS_CombatLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (CurrentTarget.IsValid())
    {
        // Check if the target can still be seen
        if (!HasLineOfSightTo(CurrentTarget.Get()))
        {
            InternalUnlock(/*bBroadcast=*/true); // Unlock if no LOS
            return;
        }

        // Check distance unlock
        if (MaxLockDistance > 0.f)
        {
            const float DistSq = FVector::DistSquared(GetOwner()->GetActorLocation(), CurrentTarget->GetActorLocation());
            if (DistSq > FMath::Square(MaxLockDistance))
            {
                InternalUnlock(/*bBroadcast=*/true);
                return;
            }
        }

        UpdateOrientation(DeltaTime); // Update orientation each tick
        UpdateReticle(GetWorld()); // Update reticle each tick
    }
}

bool UPGAS_CombatLockOnComponent::TryLockOnNearest()
{
    RebuildCandidateList();
    AActor* Best = SelectBestCandidate();
    if (!Best) return false;

    AActor* Old = CurrentTarget.Get();
    CurrentTarget = Best;

    // Bind death event of our locked on target
    if (APGAS_CharacterBase* CharTarget = Cast<APGAS_CharacterBase>(Best))
    {
        CharTarget->OnDeath.AddDynamic(this, &UPGAS_CombatLockOnComponent::HandleTargetDeath);
    }

    // Tell the animation blueprint we're locked on
    if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
    {
        if (UAnimInstance* AnimInst = Char->GetMesh() ? Char->GetMesh()->GetAnimInstance() : nullptr)
        {
            // Make sure our anim instance implements the IAnimation interface
            if (IIAnimation* AnimInterface = Cast<IIAnimation>(AnimInst))
            {
                AnimInterface->SetTargetLock(true); // Tell animation blueprint we're locked on
            }
        }
    }

    if (CachedMoveComp)
    {
        if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
        {
            // Save once if we haven't yet, then orient via controller while locked
            bSavedOrientRotationToMovement = CachedMoveComp->bOrientRotationToMovement;
            bSavedUseControllerDesiredRotation = CachedMoveComp->bUseControllerDesiredRotation;

            CachedMoveComp->bOrientRotationToMovement = false;
            CachedMoveComp->bUseControllerDesiredRotation = true;
        }
    }

    OnLocked.Broadcast(CurrentTarget.Get());
    ShowReticle();

    if (bDebugDraw)
    {
        UE_LOG(LogTemp, Warning, TEXT("LockOn: Locked to %s"), *CurrentTarget->GetName());
    }
    return true;
}

void UPGAS_CombatLockOnComponent::Unlock()
{
    InternalUnlock(/*bBroadcast=*/true);
}

bool UPGAS_CombatLockOnComponent::SwitchTargetRight()
{
    if (!IsLockedOn()) return false;
    RebuildCandidateList();

    TArray<TPair<AActor*, float>> RightSide;
    for (const TWeakObjectPtr<AActor>& C : CandidateCache)
    {
        AActor* T = C.Get();
        if (!T || T == CurrentTarget.Get()) continue;

        float SignedX = 0.f;
        if (GetScreenDirectionToTarget(T, SignedX) && SignedX > 0.f)
        {
            RightSide.Emplace(T, SignedX);
        }
    }
    if (RightSide.Num() == 0) return false;

    RightSide.Sort([ ] (const TPair<AActor*, float>& A, const TPair<AActor*, float>& B)
        {
            return A.Value < B.Value;
        });

    AActor* Old = CurrentTarget.Get();
    CurrentTarget = RightSide[0].Key;
    OnTargetChanged.Broadcast(Old, CurrentTarget.Get());

    if (bDebugDraw)
    {
        UE_LOG(LogTemp, Log, TEXT("LockOn: Switched RIGHT to %s"), *CurrentTarget->GetName());
    }
    return true;
}

bool UPGAS_CombatLockOnComponent::SwitchTargetLeft()
{
    if (!IsLockedOn()) return false;
    RebuildCandidateList();

    TArray<TPair<AActor*, float>> LeftSide;
    for (const TWeakObjectPtr<AActor>& C : CandidateCache)
    {
        AActor* T = C.Get();
        if (!T || T == CurrentTarget.Get()) continue;

        float SignedX = 0.f;
        if (GetScreenDirectionToTarget(T, SignedX) && SignedX < 0.f)
        {
            LeftSide.Emplace(T, -SignedX);
        }
    }
    if (LeftSide.Num() == 0) return false;

    LeftSide.Sort([ ] (const TPair<AActor*, float>& A, const TPair<AActor*, float>& B)
        {
            return A.Value < B.Value;
        });

    AActor* Old = CurrentTarget.Get();
    CurrentTarget = LeftSide[0].Key;
    OnTargetChanged.Broadcast(Old, CurrentTarget.Get());

    if (bDebugDraw)
    {
        UE_LOG(LogTemp, Log, TEXT("LockOn: Switched LEFT to %s"), *CurrentTarget->GetName());
    }
    return true;
}

FVector UPGAS_CombatLockOnComponent::GetAimLocation(const AActor* TargetOverride) const
{
    const AActor* T = TargetOverride ? TargetOverride : CurrentTarget.Get();
    return T ? ResolveAimLocation(T) : FVector::ZeroVector;
}

bool UPGAS_CombatLockOnComponent::GetTargetScreenPosition(UObject* WorldContextObject, FVector2D& OutScreenPos) const
{
    OutScreenPos = FVector2D::ZeroVector;
    if (!IsLockedOn()) return false;

    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
    if (!PC) return false;

    const FVector WorldPos = ResolveAimLocation(CurrentTarget.Get());
    FVector2D Screen;
    const bool bOnScreen = UGameplayStatics::ProjectWorldToScreen(PC, WorldPos, Screen, false);
    if (bOnScreen)
    {
        OutScreenPos = Screen;
    }
    return bOnScreen;
}

void UPGAS_CombatLockOnComponent::RebuildCandidateList()
{
    CandidateCache.Reset();

    FVector ViewOrigin, ViewForward;
    if (!GetViewInfo(ViewOrigin, ViewForward))
        return;

    // Gather pawns in radius
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes;
    ObjTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    TArray<AActor*> Ignore;
    Ignore.Add(GetOwner());

    // Add current target to ignore list if we have one
    if (CurrentTarget.IsValid())
    {
        Ignore.Add(CurrentTarget.Get());
    }

    // Find all actors within the lock-on radius
    TArray<AActor*> OutActors;
    const bool bFound = UKismetSystemLibrary::SphereOverlapActors(
        this,
        ViewOrigin,
        LockOnRadius,
        ObjTypes,
        TargetClassFilter ? *TargetClassFilter : APawn::StaticClass(),
        Ignore,
        OutActors
    );

    if (!bFound) return; // if none found, we exit

    // Cycle through all found actors
    for (AActor* Candidate : OutActors)
    {
        if (!Candidate || Candidate == GetOwner()) continue;

        // Calculate direction and distance to target
        const FVector ToTarget = (ResolveAimLocation(Candidate) - ViewOrigin);
        const float Dist = ToTarget.Length();
        if (Dist <= KINDA_SMALL_NUMBER) continue;

        // Normalize direction vector
        const FVector Dir = ToTarget / Dist;
        const float CosAngle = FVector::DotProduct(Dir, ViewForward);
        const float AngleDeg = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(CosAngle, -1.f, 1.f)));

        // Check if within FOV and has LOS
        if (AngleDeg <= LockOnHalfFOV && HasLineOfSightTo(Candidate))
        {
            CandidateCache.Add(Candidate);
            if (bDebugDraw)
            {
                DrawDebugSphere(GetWorld(), ResolveAimLocation(Candidate), 14.f, 8, FColor::Yellow, false, 0.05f);
            }
        }
    }

    // Sort by smallest view-angle, then distance
    CandidateCache.Sort([ & ] (const TWeakObjectPtr<AActor>& A, const TWeakObjectPtr<AActor>& B)
        {
            const FVector AO = ResolveAimLocation(A.Get()) - ViewOrigin;
            const FVector BO = ResolveAimLocation(B.Get()) - ViewOrigin;

            const float Ad = AO.Length();
            const float Bd = BO.Length();

            const float Aang = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(FVector::DotProduct(AO.GetSafeNormal(), ViewForward), -1.f, 1.f)));
            const float Bang = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(FVector::DotProduct(BO.GetSafeNormal(), ViewForward), -1.f, 1.f)));

            if (!FMath::IsNearlyEqual(Aang, Bang, 0.1f))
                return Aang < Bang;

            return Ad < Bd;
        });

    if (bDebugDraw)
    {
        const FVector End = ViewOrigin + ViewForward * FMath::Min(LockOnRadius, 600.f);
        DrawDebugLine(GetWorld(), ViewOrigin, End, FColor::Cyan, false, 0.05f, 0, 1.0f);
    }
}

AActor* UPGAS_CombatLockOnComponent::SelectBestCandidate() const
{
    return CandidateCache.Num() > 0 ? CandidateCache[0].Get() : nullptr;
}

bool UPGAS_CombatLockOnComponent::GetViewInfo(FVector& OutOrigin, FVector& OutForward) const
{
    OutOrigin = FVector::ZeroVector;
    OutForward = FVector::ForwardVector;

    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return false;

    // Prefer player camera if available
    if (APlayerController* PC = Cast<APlayerController>(OwnerActor->GetInstigatorController()))
    {
        FVector CamLoc;
        FRotator CamRot;
        PC->GetPlayerViewPoint(CamLoc, CamRot);
        OutOrigin = CamLoc;
        OutForward = CamRot.Vector();
        return true;
    }

    // Fallback: actor view (works for AI too)
    FRotator ViewRot;
    OwnerActor->GetActorEyesViewPoint(OutOrigin, ViewRot);
    OutForward = ViewRot.Vector();
    return true;
}

bool UPGAS_CombatLockOnComponent::GetScreenDirectionToTarget(const AActor* Target, float& OutSignedX) const
{
    OutSignedX = 0.f;
    if (!Target) return false;

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return false;

    FVector2D Center, TargetScreen;
    int32 SizeX, SizeY;
    PC->GetViewportSize(SizeX, SizeY);
    Center = FVector2D(SizeX * 0.5f, SizeY * 0.5f);

    if (!UGameplayStatics::ProjectWorldToScreen(PC, ResolveAimLocation(Target), TargetScreen, false))
        return false;

    OutSignedX = TargetScreen.X - Center.X; // > 0 => right side, < 0 => left side
    return true;
}

bool UPGAS_CombatLockOnComponent::HasLineOfSightTo(const AActor* Target) const
{
    if (!Target) return false;

    FVector ViewOrigin, ViewForward;
    if (!GetViewInfo(ViewOrigin, ViewForward))
        return false;

    const FVector End = ResolveAimLocation(Target);

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(LockOnLOS), false, GetOwner());
    Params.AddIgnoredActor(GetOwner());

    const bool bBlocking = GetWorld()->LineTraceSingleByChannel(Hit, ViewOrigin, End, LineOfSightChannel, Params);

    if (bDebugDraw)
    {
        const FColor Col = (!bBlocking || Hit.GetActor() == Target) ? FColor::Green : FColor::Red;
        DrawDebugLine(GetWorld(), ViewOrigin, End, Col, false, 0.05f, 0, 0.5f);
    }

    // LOS if nothing hit or we directly hit the target actor
    return !bBlocking || Hit.GetActor() == Target;
}

void UPGAS_CombatLockOnComponent::UpdateOrientation(float DeltaTime)
{
    if (!IsLockedOn()) return;

    AActor* OwnerActor = GetOwner();
    AActor* Target = CurrentTarget.Get();
    if (!OwnerActor || !Target) return;

    const FVector OwnerLoc = OwnerActor->GetActorLocation();
    const FVector AimLoc = ResolveAimLocation(Target);

    const FRotator DesiredLook = UKismetMathLibrary::FindLookAtRotation(OwnerLoc, AimLoc);

    if (bRotateControllerTowardTarget)
    {
        if (APawn* P = Cast<APawn>(OwnerActor))
        {
            if (APlayerController* PC = Cast<APlayerController>(P->GetController()))
            {
                FRotator ControlRot = PC->GetControlRotation();

                // Lock only the yaw to the target — let the player control pitch/roll
                ControlRot.Yaw = FMath::FixedTurn(
                    ControlRot.Yaw,
                    DesiredLook.Yaw,
                    RotationInterpSpeed
                );

                PC->SetControlRotation(ControlRot);
            }
        }
    }

    if (bRotateCharacterTowardTarget)
    {
        if (ACharacter* Char = Cast<ACharacter>(OwnerActor))
        {
            FRotator Current = Char->GetActorRotation();
            Current.Yaw = FMath::FixedTurn(Current.Yaw, DesiredLook.Yaw, RotationInterpSpeed);
            Char->SetActorRotation(Current);
        }
    }
}

FVector UPGAS_CombatLockOnComponent::ResolveAimLocation(const AActor* Target) const
{
    if (!Target) return FVector::ZeroVector;

    // Prefer a socket/bone on a skeletal mesh if available.
    if (const ACharacter* Char = Cast<ACharacter>(Target))
    {
        if (const USkeletalMeshComponent* Mesh = Char->GetMesh())
        {
            if (AimSocketName != NAME_None && Mesh->DoesSocketExist(AimSocketName))
            {
                return Mesh->GetSocketLocation(AimSocketName);
            }
            if (const UCapsuleComponent* Cap = Char->GetCapsuleComponent())
            {
                return Char->GetActorLocation() + FVector(0, 0, Cap->GetScaledCapsuleHalfHeight() * 0.85f);
            }
        }
    }

    // Generic fallback
    return Target->GetActorLocation() + FVector(0, 0, 80.f);
}

void UPGAS_CombatLockOnComponent::InternalUnlock(bool bBroadcast)
{
    if (!CurrentTarget.IsValid())
    {
        return;
    }

    // Unbind death event of our previously locked on target
    if (APGAS_CharacterBase* CharTarget = Cast<APGAS_CharacterBase>(CurrentTarget.Get()))
    {
        CharTarget->OnDeath.RemoveDynamic(this, &UPGAS_CombatLockOnComponent::HandleTargetDeath);
    }

    AActor* Old = CurrentTarget.Get();
    CurrentTarget.Reset();
    CandidateCache.Reset();

    // Tell the animation blueprint we're no longer locked on
    if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
    {
        if (UAnimInstance* AnimInst = Char->GetMesh() ? Char->GetMesh()->GetAnimInstance() : nullptr)
        {
            // Make sure our anim instance implements the IAnimation interface
            if (IIAnimation* AnimInterface = Cast<IIAnimation>(AnimInst))
            {
                AnimInterface->SetTargetLock(false); // Tell animation blueprint we're no longer locked on
            }
        }
    }

    if (CachedMoveComp)
    {
        CachedMoveComp->bOrientRotationToMovement = bSavedOrientRotationToMovement;
        CachedMoveComp->bUseControllerDesiredRotation = bSavedUseControllerDesiredRotation;
    }

    if (bBroadcast)
    {
        OnUnlocked.Broadcast(Old);
        if (bDebugDraw && Old)
        {
            UE_LOG(LogTemp, Log, TEXT("LockOn: Unlocked from %s"), *Old->GetName());
        }
    }

    HideReticle();
}

void UPGAS_CombatLockOnComponent::ShowReticle()
{
    if (!LockOnReticleWidgetClass) return;

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        if (!ActiveReticleWidget)
        {
            ActiveReticleWidget = CreateWidget<UUserWidget>(PC, LockOnReticleWidgetClass);
        }

        if (ActiveReticleWidget && !ActiveReticleWidget->IsInViewport())
        {
            ActiveReticleWidget->AddToViewport();
        }
    }
}

void UPGAS_CombatLockOnComponent::HideReticle()
{
    if (ActiveReticleWidget)
    {
        ActiveReticleWidget->RemoveFromParent();
        ActiveReticleWidget = nullptr;
    }
}

void UPGAS_CombatLockOnComponent::UpdateReticle(UObject* WorldContextObject)
{
    if (!ActiveReticleWidget || !IsLockedOn()) return;

    FVector2D ScreenPos;
    if (!GetTargetScreenPosition(WorldContextObject, ScreenPos)) return;

    if (UPGAS_Reticle* ReticleWidget = Cast<UPGAS_Reticle>(ActiveReticleWidget))
    {
        ReticleWidget->SetScreenPosition(ScreenPos);
    }
}

void UPGAS_CombatLockOnComponent::HandleTargetDeath()
{
    AActor* OldTarget = CurrentTarget.Get();
    if (!OldTarget) return;

    // Unbind (defensive)
    if (APGAS_CharacterBase* CharTarget = Cast<APGAS_CharacterBase>(OldTarget))
    {
        CharTarget->OnDeath.RemoveDynamic(this, &UPGAS_CombatLockOnComponent::HandleTargetDeath);
    }

    // Rebuild candidate list to find a new target
    RebuildCandidateList();
    AActor* NewTarget = SelectBestCandidate();

    if (NewTarget)
    {
        CurrentTarget = NewTarget;
        OnTargetChanged.Broadcast(OldTarget, NewTarget);
    }
    else
    {
        InternalUnlock(/*bBroadcast=*/true);
    }
}
