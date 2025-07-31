// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "MGameplayAbility.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDurationStarted);
DECLARE_MULTICAST_DELEGATE(FOnDurationRestarted);
DECLARE_MULTICAST_DELEGATE(FOnDurationEnded);

UCLASS()
class MICROCOSM_API UMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	FOnDurationStarted OnDurationStarted;
	FOnDurationRestarted OnDurationRestarted;
	FOnDurationEnded OnDurationEnded;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Input")
	FGameplayTag DefaultInputTag;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Status")
	FGameplayTag ActiveTag;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Status")
	FGameplayTag BlockedTag;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Status")
	FGameplayTag AbilityTypeTag;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Status")
	bool bRefundCooldownOnElimination = false;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bStartDurationOnActivate = true;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bReapplyDurationEffectsOnRestartDuration = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Duration")
	float Duration = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability|UI")
	bool bHideCooldownUI = false;

	/*
	 *	Abilities that should be granted alongside this one. Their activation and deactivation is potentially handled separately.
	 *	If handled by this ability, their handles should be added to SubAbilityHandles to ensure they end when this ability does.
	 *	Useful for Weapon fire modes, or reloads etc.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Ability|SubAbilities")
	TArray<TSubclassOf<UMGameplayAbility>> SubAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability|Effects")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyToSelfOnActivation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability|Effects")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyToSelfOnStartDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability|Effects")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyToSelfOnEndAbility;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/*
	 *	Used to start the duration of the ability, bStartDurationOnActivate must be either be set to false and Duration must be > 0, or must use WaitForInput task to use this.
	 *	Useful for handling preconditions such as spawning projectiles or waiting for input to start.
	 */
	UFUNCTION(BlueprintCallable)
	void StartDuration();

	UFUNCTION(BlueprintCallable)
	void RestartDuration();

protected:
	UMGameplayAbility();
	
	TArray<FGameplayAbilitySpecHandle> SubAbilityHandles;
	
	TArray<FActiveGameplayEffectHandle> ActiveOnStartEffectHandles;
	TArray<FActiveGameplayEffectHandle> ActiveOnDurationEffectHandles;
	TArray<FActiveGameplayEffectHandle> ActiveOnEndEffectHandles;

	bool bAppliedDurationEffects = false;

	UAnimInstance* GetOwnerAnimInstance() const;

	TArray<FHitResult> GetHitResultFromSweepLocationTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, float SphereSweepRadius = 30.f, bool bIgnoreSelf = true, bool bIgnoreTeam = true, bool bDrawDebug = false) const;
	
private:
	UFUNCTION()
	void EndAfterDelay();

	UPROPERTY()
	UAbilityTask_WaitDelay* WaitTask;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bRemoveOnEnd = true;
};

