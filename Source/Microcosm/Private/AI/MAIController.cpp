// copyright lhoffl

#include "AI/MAIController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BrainComponent.h"
#include "MGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AMAIController::AMAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;

	SightConfig->SetMaxAge(MaxSightAge);

	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMAIController::TargetPerceptionUpdated);
	PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AMAIController::TargetPerceptionForgotten);
}

void AMAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(const IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(InPawn))
	{
		SetGenericTeamId(1);
		ClearAndDisableAllSenses();
		EnableAllSenses();
	}
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InPawn);
	if(!ASC) return;

	ASC->RegisterGameplayTagEvent(FMGameplayTags::Get().State_Dead).AddUObject(this, &AMAIController::OnDeadTagUpdated);
}

void AMAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
}

// ReSharper disable once CppPassValueParameterByConstReference
void AMAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		if(!GetCurrentTarget()) SetCurrentTarget(Actor);
	}
	else
	{
		ForgetTargetIfDead(Actor);
	}
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void AMAIController::TargetPerceptionForgotten(AActor* Actor)
{
	if(!Actor) return;

	if(GetCurrentTarget() == Actor)
	{
		SetCurrentTarget(GetNextPerceivedActor());
	}
}

void AMAIController::OnDeadTagUpdated(const FGameplayTag GameplayTag, const int32 NewCount)
{
	if(!GameplayTag.MatchesTagExact(FMGameplayTags::Get().State_Dead)) return;

	if(NewCount > 0)
	{
		GetBrainComponent()->StopLogic(GameplayTag.ToString());
		ClearAndDisableAllSenses();
	}
	else
	{
		GetBrainComponent()->StartLogic();
		EnableAllSenses();
	}
}

UObject* AMAIController::GetCurrentTarget() const
{
	if(const UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		return BlackboardComponent->GetValueAsObject(TargetBlackboardKeyName);
	}
	return nullptr;
}

void AMAIController::SetCurrentTarget(AActor* InTarget)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if(!BlackboardComponent) return;

	if(InTarget)
	{
		BlackboardComponent->SetValueAsObject(TargetBlackboardKeyName, InTarget);
	}
	else
	{
		BlackboardComponent->ClearValue(TargetBlackboardKeyName);
	}
}

AActor* AMAIController::GetNextPerceivedActor() const
{
	if(PerceptionComponent)
	{
		TArray<AActor*> Actors;
		PerceptionComponent->GetPerceivedHostileActors(Actors);

		if(Actors.Num() != 0)
		{
			return Actors[0];
		}
	}
	return nullptr;
}

void AMAIController::ForgetTargetIfDead(AActor* Target) const
{
	const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if(!ASC) return;
	
	if(ASC->HasMatchingGameplayTag(FMGameplayTags::Get().State_Dead))
	{
		for(UAIPerceptionComponent::TActorPerceptionContainer::TIterator It = PerceptionComponent->GetPerceptualDataIterator(); It; ++It)
		{
			if(It->Key != Target) continue;

			for(FAIStimulus& Stimuli : It->Value.LastSensedStimuli)
			{
				Stimuli.SetStimulusAge(TNumericLimits<float>::Max());
			}
		}
	}
}

void AMAIController::ClearAndDisableAllSenses()
{
	PerceptionComponent->AgeStimuli(TNumericLimits<float>::Max());

	for(auto SenseConfigIt = PerceptionComponent->GetSensesConfigIterator(); SenseConfigIt; ++SenseConfigIt)
	{
		PerceptionComponent->SetSenseEnabled((*SenseConfigIt)->GetSenseImplementation(), false);
	}

	if(GetBlackboardComponent())
	{
		GetBlackboardComponent()->ClearValue(TargetBlackboardKeyName);
	}
}

void AMAIController::EnableAllSenses() const
{
	for(auto SenseConfigIt = PerceptionComponent->GetSensesConfigIterator(); SenseConfigIt; ++SenseConfigIt)
	{
		PerceptionComponent->SetSenseEnabled((*SenseConfigIt)->GetSenseImplementation(), true);
	}	
}