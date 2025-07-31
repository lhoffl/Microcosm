// copyright lhoffl


#include "Game/MGameMode.h"

#include "EngineUtils.h"
#include "GenericTeamAgentInterface.h"
#include "MGameplayTags.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "Character/MEnemyCharacter.h"
#include "Character/MPlayerCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Player/MPlayerState.h"

void AMGameMode::PlayerEliminated(AMCharacter* EliminatedCharacter, AMPlayerController* EliminatedController, AMPlayerController* AttackerController)
{
	if(!EliminatedCharacter) return;
	
	Cast<AMCharacter>(EliminatedCharacter)->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FMGameplayTags::Get().State_Dead.GetSingleTagContainer());
}

void AMGameMode::RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController)
{
	if(EliminatedCharacter)
	{
		if(AMPlayerCharacter* MPlayerCharacter = Cast<AMPlayerCharacter>(EliminatedCharacter))
		{
			EliminatedCharacter->Reset();
		}
		else if(AMEnemyCharacter* MEnemyCharacter = Cast<AMEnemyCharacter>(EliminatedCharacter))
		{
		}
	}
	if(EliminatedController && EliminatedController->IsPlayerController())
	{
		// For now, players always spawn at a predetermined spot for that player
		RestartPlayerAtPlayerStart(EliminatedController, *ControllersToStartSpots.Find(EliminatedController));
	}
}

AActor* AMGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	const IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(Player->GetPlayerState<AMPlayerState>());
	FString DesiredTag = "";

	if (TeamAgentInterface)
	{
		DesiredTag = FString::Printf(TEXT("Team_%d"), TeamAgentInterface->GetGenericTeamId().GetId());
	}

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* Start = *It;
		if (Start && Start->PlayerStartTag == FName(*DesiredTag))
		{
			UMAbilitySystemLibrary::OnScreenDebugMessage(FString::Printf(TEXT("Spawning at: %s"), *DesiredTag));
			Start->PlayerStartTag = "Taken";
			ControllersToStartSpots.Add(Player, Start);
			return Start;
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

AActor* AMGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	return ChoosePlayerStart_Implementation(Player);
}

AMGameMode::AMGameMode()
{
}