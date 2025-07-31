// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Player/MPlayerState.h"
#include "MGameState.generated.h"

UCLASS()
class AMGameState : public AGameState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_Team0)
	TArray<AMPlayerState*> Team0;

	UPROPERTY(ReplicatedUsing = OnRep_Team1)
	TArray<AMPlayerState*> Team1;

	UPROPERTY(ReplicatedUsing = OnRep_Team0Score)
	float Team0Score = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_Team1Score)
	float Team1Score = 0.f;

	UFUNCTION()
	void OnRep_Team0();

	UFUNCTION()
	void OnRep_Team1();

	UFUNCTION()
	void OnRep_Team0Score();

	UFUNCTION()
	void OnRep_Team1Score();
};	