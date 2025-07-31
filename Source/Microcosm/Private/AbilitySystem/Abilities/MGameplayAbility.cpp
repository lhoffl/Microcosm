// copyright lhoffl


#include "AbilitySystem/Abilities/MGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "MGameplayTags.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

namespace EDrawDebugTrace
{
	enum Type : int;
}

UMGameplayAbility::UMGameplayAbility()
{
	if(bRefundCooldownOnElimination)
	{
		FGameplayTagContainer Tags = GetAssetTags();
		Tags.AddTag(FMGameplayTags::Get().Abilities_RefundCooldownOnElimination);
		SetAssetTags(Tags);
	}
}

void UMGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	for(const TSubclassOf<UGameplayEffect> GameplayEffectClass : EffectsToApplyToSelfOnActivation)
	{
		const FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
		ActiveOnStartEffectHandles.AddUnique(ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, EffectSpec));
	}

	ActiveOnEndEffectHandles.Empty();
	
	if(!bStartDurationOnActivate) return;
	StartDuration();
}

void UMGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(WaitTask) WaitTask->EndTask();

	if(UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		for(const FActiveGameplayEffectHandle EffectHandle : ActiveOnStartEffectHandles) 
		{
			ASC->RemoveActiveGameplayEffect(EffectHandle);
		}
		
		for(const FActiveGameplayEffectHandle EffectHandle : ActiveOnDurationEffectHandles) 
		{
			ASC->RemoveActiveGameplayEffect(EffectHandle);
		}

		for(const TSubclassOf<UGameplayEffect> GameplayEffectClass : EffectsToApplyToSelfOnEndAbility)
		{
			const FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
			ActiveOnEndEffectHandles.AddUnique(ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, EffectSpec));
		}

		ASC->RemoveLooseGameplayTag(ActiveTag);
	}

	ActiveOnDurationEffectHandles.Empty();
	ActiveOnStartEffectHandles.Empty();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if(!ASC) return;
	
	for(FGameplayAbilitySpecHandle SubAbilityHandle : SubAbilityHandles)
	{
		ASC->ClearAbility(SubAbilityHandle);
	}

	for(const FActiveGameplayEffectHandle EffectHandle : ActiveOnStartEffectHandles) 
	{
		ASC->RemoveActiveGameplayEffect(EffectHandle);
	}
		
	for(const FActiveGameplayEffectHandle EffectHandle : ActiveOnDurationEffectHandles) 
	{
		ASC->RemoveActiveGameplayEffect(EffectHandle);
	}

	for(const FActiveGameplayEffectHandle EffectHandle : ActiveOnEndEffectHandles)
	{
		ASC->RemoveActiveGameplayEffect(EffectHandle);
	}
}

void UMGameplayAbility::StartDuration()
{
	if(WaitTask && WaitTask->IsActive()) return;
	
	if(UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if(!ASC->HasMatchingGameplayTag(ActiveTag)) ASC->AddLooseGameplayTag(ActiveTag);
		if(!bAppliedDurationEffects || bReapplyDurationEffectsOnRestartDuration)
		{
			for(const TSubclassOf<UGameplayEffect> GameplayEffectClass : EffectsToApplyToSelfOnStartDuration)
			{
				const FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
				ActiveOnDurationEffectHandles.Add(ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), EffectSpec));
			}
			
			bAppliedDurationEffects = true;
		}
		
		if(Duration > 0.f)
		{
			if(OnDurationStarted.IsBound()) OnDurationStarted.Broadcast();
			WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, Duration);
			if (WaitTask)
			{
				WaitTask->OnFinish.AddDynamic(this, &UMGameplayAbility::EndAfterDelay);
				WaitTask->ReadyForActivation();
			}
		}

		if(bRemoveOnEnd)
		{
			ASC->SetRemoveAbilityOnEnd(CurrentSpecHandle);
		}
	}	
}

void UMGameplayAbility::RestartDuration()
{
	if(WaitTask && WaitTask->IsActive())
	{
		WaitTask->OnFinish.RemoveAll(this);
		WaitTask->EndTask();
		StartDuration();

		if(OnDurationRestarted.IsBound()) OnDurationRestarted.Broadcast();
	}
}

UAnimInstance* UMGameplayAbility::GetOwnerAnimInstance() const
{
	if(const USkeletalMeshComponent* OwnerMesh = GetOwningComponentFromActorInfo())
	{
		return OwnerMesh->GetAnimInstance();
	}

	return nullptr;
}

TArray<FHitResult> UMGameplayAbility::GetHitResultFromSweepLocationTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, const float SphereSweepRadius, const bool bIgnoreSelf, const bool bIgnoreTeam, const bool bDrawDebug) const
{
	TArray<FHitResult> OutResult;
	TSet<AActor*> HitActors;
	
	for(const TSharedPtr<FGameplayAbilityTargetData> TargetData : TargetDataHandle.Data)
	{
		const FVector StartLocation = TargetData->GetOrigin().GetTranslation();
		const FVector EndLocation = TargetData->GetEndPoint();

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		TArray<AActor*> ActorsToIgnore;
		if(bIgnoreSelf) ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
		if(bIgnoreTeam) ActorsToIgnore.Append(UMAbilitySystemLibrary::GetAllPlayersOnTeamAsActors(GetAvatarActorFromActorInfo()));

		const EDrawDebugTrace::Type DrawDebug = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		TArray<FHitResult> Results;
		UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartLocation, EndLocation, SphereSweepRadius, ObjectTypes, false, ActorsToIgnore, DrawDebug, Results, false);

		for(const FHitResult& Result : Results)
		{
			if(UMAbilitySystemLibrary::IsFriend(Result.GetActor(), GetAvatarActorFromActorInfo())) continue;
			if(HitActors.Contains(Result.GetActor())) continue;
			
			HitActors.Add(Result.GetActor());
			OutResult.Add(Result);
		}
	}

	return OutResult;
}

void UMGameplayAbility::EndAfterDelay()
{
	if(OnDurationEnded.IsBound()) OnDurationEnded.Broadcast();
	//EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}