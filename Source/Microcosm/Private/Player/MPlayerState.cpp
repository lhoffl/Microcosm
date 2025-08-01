// copyright lhoffl


#include "Player/MPlayerState.h"

#include "AbilitySystem/MAbilitySystemComponent.h"
#include "AbilitySystem/MAttributeSet.h"

AMPlayerState::AMPlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UMAttributeSet>("AttributeSet");
}

void AMPlayerState::BeginPlay()
{
	Super::BeginPlay();
	SetGenericTeamId(0);
}

UAbilitySystemComponent* AMPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AMPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

FGenericTeamId AMPlayerState::GetGenericTeamId() const
{
	return TeamID;
}

void AMPlayerState::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	TeamID = InTeamID;

	/*if(AMPlayerCharacter* MCharacter = Cast<AMPlayerCharacter>(GetPawn()))
	{
		MCharacter->SetGenericTeamId(TeamID);
	}*/
}

ETeamAttitude::Type AMPlayerState::GetTeamAttitudeTowards(const AActor& Other) const
{
	return IGenericTeamAgentInterface::GetTeamAttitudeTowards(Other);
}