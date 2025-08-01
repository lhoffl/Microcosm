// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h"
#include "Perception/AIPerceptionTypes.h"
#include "MAIController.generated.h"

UCLASS()
class AMAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI|Behavior")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Behavior")
	FName TargetBlackboardKeyName;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI|Perception|Sight")
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI|Perception|Sight")
	float SightRadius = 1000.f;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI|Perception|Sight")
	float LoseSightRadius = 1200.f;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI|Perception|Sight")
	float MaxSightAge = 5.f;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "AI|Perception|Sight")
	float PeripheralVisionAngleDegrees = 100.f;

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void TargetPerceptionForgotten(AActor* Actor);

	void OnDeadTagUpdated(FGameplayTag GameplayTag, int32 NewCount);

	UObject* GetCurrentTarget() const;
	void SetCurrentTarget(AActor* InTarget);

	AActor* GetNextPerceivedActor() const;

	void ForgetTargetIfDead(AActor* Target) const;

	void ClearAndDisableAllSenses();
	void EnableAllSenses() const;
};
