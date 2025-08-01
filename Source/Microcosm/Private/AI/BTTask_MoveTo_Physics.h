// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveTo_Physics.generated.h"

UCLASS()
class UBTTask_MoveTo_Physics : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Target")
	FName TargetName;

	UPROPERTY(EditAnywhere, Category = "Target")
	float MoveForce = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Target")
	bool bRotateToTarget = true;

	UPROPERTY(EditAnywhere, Category = "Target")
	float AcceptableRadius = 100.f;
};
