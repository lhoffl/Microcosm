// copyright lhoffl


#include "AI/BTTask_SendInputToAbilitySystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "AbilitySystem/MAbilitySystemComponent.h"

EBTNodeResult::Type UBTTask_SendInputToAbilitySystem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* OwnerAIController = OwnerComp.GetAIOwner();
	if(!OwnerAIController) return EBTNodeResult::Failed;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerAIController->GetPawn());
	if(!ASC) return EBTNodeResult::Failed;

	UMAbilitySystemComponent* MASC = Cast<UMAbilitySystemComponent>(ASC);
	if(!MASC) return EBTNodeResult::Failed;

	switch (InputEvent)
	{
	case Pressed:
		MASC->AbilityInputTagPressed(InputTag);
		break;

	case Held:
		MASC->AbilityInputTagHeld(InputTag);
		break;

	case Released:
		MASC->AbilityInputTagReleased(InputTag);
		break;
		
	default: return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Succeeded;
}
