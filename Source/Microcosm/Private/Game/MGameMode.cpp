// copyright lhoffl


#include "Game/MGameMode.h"

#include "MGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AMGameMode::AdvanceToNextLevel() const
{
	const FString LevelName = "Level" + FString::FromInt(LevelIndex);
	UGameplayStatics::OpenLevel(this, FName(*LevelName), true);
}

void AMGameMode::GoalReached(const EGoalType GoalType)
{
	GetWorldTimerManager().SetTimer(DelayUntilNextLevelTimerHandle, this, &AMGameMode::AdvanceToNextLevel, DelayAfterGoalBeforeNextLevel);
	OnGoalReached.Broadcast(GoalType);

	switch (GoalType)
	{
	case EGoalType::Standard:
		LevelIndex++;
		break;
	case EGoalType::Skip:
		LevelIndex += 5;
		break;
	}

	if (UMGameInstance* GI = Cast<UMGameInstance>(GetGameInstance()))
	{
		GI->LevelIndex = LevelIndex;
	}
}

void AMGameMode::RestartLevel() const
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}

void AMGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(LoopTickTimerHandle, this, &AMGameMode::AdvanceLoop, LoopTickRate, true);
	GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &AMGameMode::RestartLevel, LevelTimer);

	TArray<AActor*> GoalsAsActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMGoalActor::StaticClass(), GoalsAsActors);
	for(AActor* Goal : GoalsAsActors)
	{
		if(AMGoalActor* GoalActor = Cast<AMGoalActor>(Goal))
		{
			GoalActors.Add(GoalActor);
			GoalActor->OnGoalReached.AddDynamic(this, &AMGameMode::GoalReached);
		}
	}

	if (UMGameInstance* GI = Cast<UMGameInstance>(GetGameInstance()))
	{
		LevelIndex = GI->LevelIndex;
		GI->PlayMusic();
	}
}

void AMGameMode::AdvanceLoop()
{
	CurrentTick += 1;
	CurrentTick = FMath::Clamp(CurrentTick, 0, std::numeric_limits<int32>::max());
	OnLoopTickIncreased.Broadcast(CurrentTick);
}