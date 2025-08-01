// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SendInputToAbilitySystem.generated.h"

UENUM()
enum EAbilitySystemInputEvent
{
	None,
	Pressed,
	Held,
	Released
};

UCLASS()
class UBTTask_SendInputToAbilitySystem : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TEnumAsByte<EAbilitySystemInputEvent> InputEvent = None;
};
