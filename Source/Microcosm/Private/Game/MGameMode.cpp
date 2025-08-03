// copyright lhoffl


#include "Game/MGameMode.h"

#include "MGameInstance.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "Character/MPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void AMGameMode::AdvanceToNextLevel() const
{
	const FString LevelName = FString::Printf(TEXT("/Game/_Blueprints/Maps/Level%d"), LevelIndex);
	UE_LOG(LogTemp, Warning, TEXT("Trying to open level: %s"), *LevelName);
	UGameplayStatics::OpenLevel(this, FName(*LevelName), true);
}

void AMGameMode::GoalReached(const EGoalType GoalType)
{
	if (UMGameInstance* GI = Cast<UMGameInstance>(GetGameInstance())) {
		const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
		float EndTime = GI->GetElapsedPlayTime() - StartTime;
		GI->LevelNamesToTime.Add(FName(LevelName), EndTime);
	}
	
	OnGoalReached.Broadcast(GoalType);

	GetWorldTimerManager().SetTimer(DelayUntilNextLevelTimerHandle, this, &AMGameMode::AdvanceToNextLevel, DelayAfterGoalBeforeNextLevel);

	if (UMGameInstance* GI = Cast<UMGameInstance>(GetGameInstance()))
	{
		switch (GoalType)
		{
		case EGoalType::Standard:
			LevelIndex++;
			//GI->PlaySFX(LevelComplete);
			break;
		case EGoalType::Skip:
			LevelIndex += 5;
			GI->PlaySFX(LevelSkipComplete);
			break;
		case EGoalType::LastGoalForWorld:
			DelayUntilNextLevelTimerHandle.Invalidate();
		}
		
		GI->PlaySFX(LevelCompleteJingle);
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
		StartTime = GI->GetElapsedPlayTime();
	}
}

void AMGameMode::AdvanceLoop()
{
	CurrentTick += 1;
	CurrentTick = FMath::Clamp(CurrentTick, 0, std::numeric_limits<int32>::max());
	OnLoopTickIncreased.Broadcast(CurrentTick);
}