// copyright lhoffl

#pragma once

#include "MAbilitySystemLibrary.h"

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"
#include "MAbilityTypes.h"
#include "MGameplayTags.h"
#include "Abilities/GameplayAbility.h"
#include "Character/MCharacter.h"
#include "Game/MGameMode.h"
#include "Game/MGameState.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MPlayerState.h"

UCharacterClassInfo* UMAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AMGameMode* MGameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	
	if(!MGameMode) return nullptr;
	
	return MGameMode->CharacterClassInfo;
}

bool UMAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FMGameplayEffectContext* MGameplayEffectContext = static_cast<const FMGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MGameplayEffectContext->IsBlockedHit();
	}
	return false;
}

void UMAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if(FMGameplayEffectContext* MGameplayEffectContext = static_cast<FMGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MGameplayEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

bool UMAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FMGameplayEffectContext* MGameplayEffectContext = static_cast<const FMGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MGameplayEffectContext->IsCriticalHit();
	}
	return false;	
}

void UMAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsCriticalHit)
{
	if(FMGameplayEffectContext* MGameplayEffectContext = static_cast<FMGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MGameplayEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UMAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	/*FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if(const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for(FOverlapResult& Overlap : Overlaps)
		{
			if(Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
	}*/
}

bool UMAbilitySystemLibrary::IsFriend(AActor* FirstActor, const AActor* SecondActor)
{
	if(!FirstActor || !SecondActor) return false;
	if(FirstActor == SecondActor) return true;
	
	const IGenericTeamAgentInterface* SourceTeamAgentInterface = Cast<IGenericTeamAgentInterface>(FirstActor);
	return SourceTeamAgentInterface->GetTeamAttitudeTowards(*SecondActor) != ETeamAttitude::Hostile;
}

FGameplayEffectContextHandle UMAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FMGameplayTags& GameplayTags = FMGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return  EffectContextHandle;
}

TArray<FRotator> UMAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;

	if(NumRotators > 1)
	{
		const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	
		const float DeltaSpread = Spread / (NumRotators - 1);
		for(int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}

	return Rotators;
}

TArray<FVector> UMAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;

	if(NumVectors > 1)
	{
		const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	
		const float DeltaSpread = Spread / (NumVectors - 1);
		for(int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}

	return Vectors;
}

TArray<FVector> UMAbilitySystemLibrary::EvenlyRotatedVectorsArc(const FVector& Forward, const float AngleDegrees, const int32 NumVectors)
{
	TArray<FVector> Result;

	if (NumVectors <= 0 || Forward.IsNearlyZero())
		return Result;

	const FVector ForwardXZ = FVector(Forward.X, 0, Forward.Z).GetSafeNormal();

	const float HalfAngleRad = FMath::DegreesToRadians(AngleDegrees / 2.0f);

	for (int32 i = 0; i < NumVectors; ++i)
	{
		float T = (NumVectors > 1) ? static_cast<float>(i) / (NumVectors - 1) : 0.5f;
		const float AngleOffsetRad = FMath::Lerp(-HalfAngleRad, HalfAngleRad, T);

		const float CosAngle = FMath::Cos(AngleOffsetRad);
		const float SinAngle = FMath::Sin(AngleOffsetRad);

		// Manual 2D rotation in XZ plane (around Y)
		FVector Rotated = FVector(
			ForwardXZ.X * CosAngle - ForwardXZ.Z * SinAngle,
			0,
			ForwardXZ.X * SinAngle + ForwardXZ.Z * CosAngle
		);

		Result.Add(Rotated.GetSafeNormal());
	}

	return Result;
}

FGameplayAbilitySpecHandle UMAbilitySystemLibrary::TryActivateAbilityFromSpec(UAbilitySystemComponent* ASC, const TSubclassOf<UGameplayAbility> Ability, bool& bWasSuccessful, bool bRequireAuthority)
{
	if (!ASC) return FGameplayAbilitySpecHandle();
	//if(!ASC->GetAvatarActor()->HasAuthority() && bRequireAuthority) return FGameplayAbilitySpecHandle(); 
	
	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.Ability->GetClass() == Ability)
		{
			bWasSuccessful = ASC->TryActivateAbility(Spec.Handle, true);
			return Spec.Handle;
		}
	}

	bWasSuccessful = false;
	return FGameplayAbilitySpecHandle();	
}

FGenericTeamId UMAbilitySystemLibrary::GetTeam(AActor* InActor)
{
	if(!InActor) return FGenericTeamId::NoTeam;
	
	if(const AMCharacter* MCharacter = Cast<AMCharacter>(InActor))
	{
		if(const AMPlayerState* SourcePlayerState = Cast<AMPlayerState>(MCharacter->GetPlayerState()))
		{
			return SourcePlayerState->GetGenericTeamId();
		}
	}

	return FGenericTeamId::NoTeam;
}

FGameplayEffectSpecHandle UMAbilitySystemLibrary::MakeSetByCallerEffectSpec(UAbilitySystemComponent* ASC, const TSubclassOf<UGameplayEffect>& EffectClass, const FGameplayTag& MagnitudeTag, const float MagnitudeAmount, const float Level)
{
	if (!ASC) return FGameplayEffectSpecHandle();
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, Level, EffectContextHandle);
	return UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, MagnitudeTag, MagnitudeAmount);	
}

FActiveGameplayEffectHandle UMAbilitySystemLibrary::ApplySetByCallerEffectSpec(UAbilitySystemComponent* ASC, const TSubclassOf<UGameplayEffect>& EffectClass, const FGameplayTag& MagnitudeTag, const float MagnitudeAmount, const float Level)
{
	if(!ASC) return FActiveGameplayEffectHandle();
	const FGameplayEffectSpecHandle SpecHandle = MakeSetByCallerEffectSpec(ASC, EffectClass, MagnitudeTag, MagnitudeAmount, Level);
	return ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

FActiveGameplayEffectHandle UMAbilitySystemLibrary::ApplyInstantDynamicEffect(
	UAbilitySystemComponent* ASC,
	const FGameplayAttribute& AttributeToModify,
	const float ModifierMagnitude,
	const EModOp& ModOp,
	const FName& EffectName)
{
	UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), EffectName);
	GameplayEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

	const int32 Index = GameplayEffect->Modifiers.Num();
	GameplayEffect->Modifiers.SetNum(Index + 1);
	FGameplayModifierInfo& Info = GameplayEffect->Modifiers[Index];
	Info.ModifierMagnitude = FScalableFloat(ModifierMagnitude);

	switch (ModOp) {
	case Additive:
		Info.ModifierOp = EGameplayModOp::Additive;
		break;
	case Multiplicitive:
		Info.ModifierOp = EGameplayModOp::Multiplicitive;
		break;
	case Division:
		Info.ModifierOp = EGameplayModOp::Division;
		break;
	case Max:
		Info.ModifierOp = EGameplayModOp::Max;
		break;
	case Override:
		Info.ModifierOp = EGameplayModOp::Multiplicitive;
		break;
	}
	
	Info.Attribute = AttributeToModify;
		
	return ASC->ApplyGameplayEffectToSelf(GameplayEffect, 1.0f, ASC->MakeEffectContext());
}

void UMAbilitySystemLibrary::ApplyOrRefreshTagForDuration(
	UAbilitySystemComponent* ASC,
	const FGameplayTag& Tag,
	float Duration,
	FTimerHandle& TimerHandle)
{
	if (!ASC || !Tag.IsValid()) return;

	if (!ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->AddLooseGameplayTag(Tag);
	}

	TWeakObjectPtr<UAbilitySystemComponent> WeakPtrASC = ASC;

	UWorld* World = ASC->GetWorld(); 
	if (!World) return;

	FTimerManager& TimerManager = World->GetTimerManager();

	if (TimerManager.IsTimerActive(TimerHandle))
	{
		TimerManager.ClearTimer(TimerHandle);
	}

	TimerManager.SetTimer(TimerHandle, [WeakPtrASC, Tag]()
	{
		if (UAbilitySystemComponent* InnerStrongPtrASC = WeakPtrASC.Get())
		{
			InnerStrongPtrASC->RemoveLooseGameplayTag(Tag);
		}
	}, Duration, false);
}

void UMAbilitySystemLibrary::ApplyTagForDuration(UAbilitySystemComponent* ASC, const FGameplayTag& Tag, float Duration)
{
	if (!ASC || !Tag.IsValid()) return;

	ASC->AddLooseGameplayTag(Tag);

	TWeakObjectPtr<UAbilitySystemComponent> WeakPtrASC = ASC;

	ASC->GetWorld()->GetTimerManager().SetTimerForNextTick([WeakPtrASC, Tag, Duration]()
	{
		const UAbilitySystemComponent* StrongPtrASC = WeakPtrASC.Get();
	
		if (!StrongPtrASC) return;

		FTimerHandle TimerHandle;
		StrongPtrASC->GetWorld()->GetTimerManager().SetTimer(TimerHandle,[WeakPtrASC, Tag]()
		{
			if (UAbilitySystemComponent* InnerStrongPtrASC = WeakPtrASC.Get())
			{
				InnerStrongPtrASC->RemoveLooseGameplayTag(Tag);
			}
			},
			Duration,
			false
		);
	});
}

void UMAbilitySystemLibrary::OnScreenDebugMessage(const FString& Message, const float Time, const FColor Color, const int32 Key)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Key, Time, Color, Message);
	}
}

void UMAbilitySystemLibrary::DebugMessage(const FString& Message)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
}

float UMAbilitySystemLibrary::GetMaxMoveSpeedForCurrentGameMode(const UObject* WorldContextObject)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		if (const AMGameMode* GM = Cast<AMGameMode>(World->GetAuthGameMode()))
		{
			return GM->GetMaxPlayerMoveSpeed();
		}
	}
	return 0.f;	
}

TArray<FActiveGameplayEffectHandle> UMAbilitySystemLibrary::ApplyEffectsToTarget(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const TArray<TSubclassOf<UGameplayEffect>>& EffectsToApplyToTarget, const float Level)
{
	TArray<FActiveGameplayEffectHandle> EffectHandles;
	AActor* SourceAvatarActor = SourceASC->GetAvatarActor();
	if(!SourceAvatarActor->HasAuthority()) return EffectHandles;
	for(const TSubclassOf<UGameplayEffect> EffectClass : EffectsToApplyToTarget)
	{
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.AddInstigator(SourceAvatarActor, SourceAvatarActor);
		FGameplayEffectSpecHandle EffectSpec = SourceASC->MakeOutgoingSpec(EffectClass, Level, EffectContext);
		EffectHandles.Add(SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data, TargetASC));
	}
	return EffectHandles;
}
