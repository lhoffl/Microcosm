// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "MEnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "MEnemyContainer.generated.h"

UCLASS()
class AMEnemyContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	AMEnemyContainer();
	virtual void Tick(float DeltaTime) override;
	
protected:
	UFUNCTION()
	void OnLoopTickIncreased(int32 Tick);
	void UpdateCurrentEnemy();
	void BindToGameModeDelegate();
	UFUNCTION()
	void OnEnemyDestroyed(AMEnemyCharacter* EnemyCharacter);
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AMEnemyCharacter>> EnemyCharactersClasses;

	int CurrentEnemyIndex = 0;

	UPROPERTY()
	AMEnemyCharacter* CurrentEnemy;
	
	UPROPERTY()
	TArray<AMEnemyCharacter*> EnemyCharacters;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* EnemyDefeated;
};
