// copyright lhoffl


#include "AI/BTTask_MoveTo_Physics.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Character/MEnemyCharacter.h"

EBTNodeResult::Type UBTTask_MoveTo_Physics::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(TargetName.IsNone()) return EBTNodeResult::Failed;

	const AAIController* OwnerAIController = OwnerComp.GetAIOwner();
	if(!OwnerAIController) return EBTNodeResult::Failed;

	APawn* OwnerPawn = OwnerAIController->GetPawn();
	if(!OwnerPawn) return EBTNodeResult::Failed;

	AMEnemyCharacter* EnemyCharacter = Cast<AMEnemyCharacter>(OwnerPawn);
	if(!EnemyCharacter) return EBTNodeResult::Failed;

	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(EnemyCharacter->GetRootComponent());
	if(!StaticMeshComponent) return EBTNodeResult::Failed;

	const AActor* TargetActor = Cast<AActor>(OwnerAIController->GetBlackboardComponent()->GetValueAsObject(TargetName));	
	if(!TargetActor) return EBTNodeResult::Failed;

	FVector Direction = TargetActor->GetActorLocation() - EnemyCharacter->GetActorLocation();
	Direction.Normalize();

	if(bRotateToTarget) EnemyCharacter->FaceRotation(Direction.Rotation());

	const float DistanceFromTarget = FVector::Distance(TargetActor->GetActorLocation(), EnemyCharacter->GetActorLocation());
	if(DistanceFromTarget <= AcceptableRadius) return EBTNodeResult::Failed;
	
	const FVector Impulse = Direction * MoveForce;
	StaticMeshComponent->AddImpulse(Impulse, NAME_None, true);
	
	return EBTNodeResult::Succeeded;
}
