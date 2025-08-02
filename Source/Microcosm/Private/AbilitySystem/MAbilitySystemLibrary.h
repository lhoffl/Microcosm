// copyright lhoffl
#pragma once

#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "MAbilityTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Player/MPlayerState.h"

#include "MAbilitySystemLibrary.generated.h"

UENUM()
enum EModOp
{
	Additive,
	Multiplicitive,
	Division,
	Max,
	Override
};

UCLASS()
class MICROCOSM_API UMAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//UFUNCTION(BlueprintPure, Category = "MAbilitySystemLibrary|WidgetController")
	//static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	//UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|CharacterClassDefaults")
	//static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	//UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|CharacterClassDefaults")
	//static void GiveStartUpAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "MAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintPure, Category = "MAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, UPARAM(ref) TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static bool IsFriend(AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintPure, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);
	
	UFUNCTION(BlueprintPure, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);

	UFUNCTION(BlueprintPure, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlyRotatedVectorsArc(const FVector& Forward, const float AngleDegrees, const int32 NumVectors);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static FGameplayAbilitySpecHandle TryActivateAbilityFromSpec(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayAbility> Ability, bool& bWasSuccessful, bool bRequireAuthority = true);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|Teams")
	static FGenericTeamId GetTeam(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static FGameplayEffectSpecHandle MakeSetByCallerEffectSpec(UAbilitySystemComponent* ASC, const TSubclassOf<UGameplayEffect>& EffectClass, const FGameplayTag& MagnitudeTag, const float MagnitudeAmount, const float Level = 1.f);
	
	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static FActiveGameplayEffectHandle ApplySetByCallerEffectSpec(UAbilitySystemComponent* ASC, const TSubclassOf<UGameplayEffect>& EffectClass, const FGameplayTag& MagnitudeTag, const float MagnitudeAmount, const float Level = 1.f);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static FActiveGameplayEffectHandle ApplyInstantDynamicEffect(
		UAbilitySystemComponent* ASC,
		const FGameplayAttribute& AttributeToModify,
		const float ModifierMagnitude,
		const EModOp& ModOp = EModOp::Additive,
		const FName& EffectName = FName("DynamicEffect")
		);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static void ApplyOrRefreshTagForDuration(UAbilitySystemComponent* ASC, const FGameplayTag& Tag, float Duration, FTimerHandle& TimerHandle);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|GameplayMechanics")
	static void ApplyTagForDuration(UAbilitySystemComponent* ASC, const FGameplayTag& Tag, float Duration);

	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|Debug")
	static void OnScreenDebugMessage(const FString& Message, float Time = 2.f, FColor Color = FColor::Green, int32 Key = -1);
	
	UFUNCTION(BlueprintCallable, Category = "MAbilitySystemLibrary|Debug")
	static void DebugMessage(const FString& Message);

	static float GetMaxMoveSpeedForCurrentGameMode(const UObject* WorldContextObject);

	static TArray<FActiveGameplayEffectHandle> ApplyEffectsToTarget(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const TArray<TSubclassOf<UGameplayEffect>>& EffectsToApplyToTarget, float Level = 1.f);
};