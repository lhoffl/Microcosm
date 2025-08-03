// copyright lhoffl


#include "Character/MEnemyContainer.h"

#include "MEnemyCharacter.h"
#include "Game/MGameMode.h"
#include "Kismet/GameplayStatics.h"

AMEnemyContainer::AMEnemyContainer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMEnemyContainer::BeginPlay()
{
	Super::BeginPlay();

	for(TSubclassOf<AMEnemyCharacter> EnemyCharacterClass : EnemyCharactersClasses)
	{
		AMEnemyCharacter* EnemyCharacter = GetWorld()->SpawnActorDeferred<AMEnemyCharacter>(EnemyCharacterClass, GetTransform());
		EnemyCharacter->FinishSpawning(GetTransform());
		EnemyCharacter->ToggleActive(false);
		EnemyCharacters.Add(EnemyCharacter);
		EnemyCharacter->OnDeathStarted.AddDynamic(this, &AMEnemyContainer::OnEnemyDestroyed);
	}

	if(EnemyCharacters.Num() > 0)
	{
		CurrentEnemy = EnemyCharacters[CurrentEnemyIndex];
		if(CurrentEnemy) CurrentEnemy->ToggleActive(true);
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMEnemyContainer::BindToGameModeDelegate);
}

void AMEnemyContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMEnemyContainer::OnLoopTickIncreased(int32 Tick)
{
	UpdateCurrentEnemy();
}

void AMEnemyContainer::UpdateCurrentEnemy()
{
	if(EnemyCharacters.Num() <= 1) return;
	AMEnemyCharacter* PreviousEnemy = CurrentEnemy;
	if(CurrentEnemy) PreviousEnemy->ToggleActive(false);

	CurrentEnemyIndex++;
	CurrentEnemyIndex %= EnemyCharacters.Num();
	CurrentEnemy = EnemyCharacters[CurrentEnemyIndex];
	if(CurrentEnemy) CurrentEnemy->ToggleActive(true);
}

void AMEnemyContainer::BindToGameModeDelegate()
{
	if (AMGameMode* GM = Cast<AMGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnLoopTickIncreased.AddDynamic(this, &AMEnemyContainer::OnLoopTickIncreased);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMEnemyContainer::BindToGameModeDelegate);
	}
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void AMEnemyContainer::OnEnemyDestroyed(AMEnemyCharacter* EnemyCharacter)
{
	for(AMEnemyCharacter* Enemy : EnemyCharacters)
	{
		if(Enemy == EnemyCharacter) continue;
		Enemy->Destroy();
	}

	if (UMGameInstance* GI = Cast<UMGameInstance>(GetGameInstance()))
	{
		GI->PlaySFX(EnemyDefeated);
	}
	
	Destroy();
}
