// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Actor/MGoalActor.h"
#include "Character/MCharacter.h"
#include "GameFramework/GameMode.h"
#include "MGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoopTickIncreased, int32, Tick);

UCLASS()
class AMGameMode : public AGameMode
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Info")
	UCharacterClassInfo* CharacterClassInfo;

	UPROPERTY(BlueprintAssignable)
	FOnLoopTickIncreased OnLoopTickIncreased;

	UPROPERTY(BlueprintAssignable)
	FOnGoalReached OnGoalReached;
	FTimerHandle LevelTimerHandle;

	float GetMaxPlayerMoveSpeed() const { return MaxPlayerMoveSpeed; }

	UFUNCTION(BlueprintCallable)
	void RestartLevel() const;
	
	UFUNCTION(BlueprintCallable)
	float GetLoopTickRate() const { return LoopTickRate; }

	UFUNCTION(BlueprintCallable)
	float GetLevelTimer() const { return LevelTimer; }
	
private:
	void AdvanceToNextLevel() const;
	UFUNCTION()
	void GoalReached(EGoalType GoalType);
	virtual void BeginPlay() override;

	FTimerHandle DelayUntilNextLevelTimerHandle;
	UPROPERTY(EditDefaultsOnly)
	float DelayAfterGoalBeforeNextLevel = 0.6f;
	
	UPROPERTY()
	TMap<AController*, APlayerStart*> ControllersToStartSpots;

	UPROPERTY(EditDefaultsOnly)
	float MaxPlayerMoveSpeed;

	UFUNCTION()
	void AdvanceLoop();
	
	UPROPERTY(EditDefaultsOnly)
	float LoopTickRate = 1.2f;

	int32 CurrentTick = 0;
	FTimerHandle LoopTickTimerHandle;

	int LevelIndex = 1;
	UPROPERTY()
	TArray<AMGoalActor*> GoalActors;
	
	float LevelTimer = 60.f;
};
