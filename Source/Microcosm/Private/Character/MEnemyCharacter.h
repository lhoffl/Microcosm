#pragma once
#include "MCharacter.h"
#include "MEnemyCharacter.generated.h"

UCLASS()
class AMEnemyCharacter : public AMCharacter
{
	GENERATED_BODY()

public:
	AMEnemyCharacter();
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	bool IsDead() const;
	void Revive();
};
