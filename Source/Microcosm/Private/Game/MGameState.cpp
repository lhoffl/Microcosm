// copyright lhoffl


#include "Game/MGameState.h"

#include "Net/UnrealNetwork.h"

void AMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMGameState, Team0);
	DOREPLIFETIME(AMGameState, Team1);
	DOREPLIFETIME(AMGameState, Team0Score);
	DOREPLIFETIME(AMGameState, Team1Score);
}

void AMGameState::OnRep_Team0()
{
}

void AMGameState::OnRep_Team1()
{
}

void AMGameState::OnRep_Team0Score()
{
	
}

void AMGameState::OnRep_Team1Score()
{
	
}