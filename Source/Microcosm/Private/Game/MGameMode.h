// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Character/MCharacter.h"
#include "GameFramework/GameMode.h"
#include "Player/MPlayerController.h"
#include "MGameMode.generated.h"

UCLASS()
class AMGameMode : public AGameMode
{
	GENERATED_BODY()
public:

	AMGameMode();

	UPROPERTY(EditDefaultsOnly, Category = "Character Info")
	UCharacterClassInfo* CharacterClassInfo;
	
	virtual void PlayerEliminated(AMCharacter* EliminatedCharacter, AMPlayerController* EliminatedController, AMPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

	float GetMaxPlayerMoveSpeed() const { return MaxPlayerMoveSpeed; }
	
private:
	UPROPERTY()
	TMap<AController*, APlayerStart*> ControllersToStartSpots;

	UPROPERTY(EditDefaultsOnly)
	float MaxPlayerMoveSpeed;	
};
