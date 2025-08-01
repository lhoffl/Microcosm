// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Actor/MGoalActor.h"
#include "Character/MCharacter.h"
#include "GameFramework/GameMode.h"
#include "Player/MPlayerController.h"
#include "MGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoopTickIncreased, int)

UCLASS()
class AMGameMode : public AGameMode
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Info")
	UCharacterClassInfo* CharacterClassInfo;

	FOnLoopTickIncreased OnLoopTickIncreased;

	UPROPERTY(BlueprintAssignable)
	FOnGoalReached OnGoalReached;

	virtual void PlayerEliminated(AMCharacter* EliminatedCharacter, AMPlayerController* EliminatedController, AMPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

	float GetMaxPlayerMoveSpeed() const { return MaxPlayerMoveSpeed; }
	
private:
	void AdvanceToNextLevel();
	UFUNCTION()
	void GoalReached(EGoalType GoalType);
	virtual void BeginPlay() override;

	FTimerHandle DelayUntilNextLevelTimerHandle;
	UPROPERTY(EditDefaultsOnly)
	float DelayAfterGoalBeforeNextLevel = 5.f;
	
	UPROPERTY()
	TMap<AController*, APlayerStart*> ControllersToStartSpots;

	UPROPERTY(EditDefaultsOnly)
	float MaxPlayerMoveSpeed;

	UFUNCTION()
	void AdvanceLoop();
	
	UPROPERTY(EditDefaultsOnly)
	float LoopTickRate = 1.f;

	int32 CurrentTick = 0;
	FTimerHandle LoopTickTimerHandle;

	int LevelIndex = 1;
	UPROPERTY()
	TArray<AMGoalActor*> GoalActors;
};
