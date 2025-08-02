// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

UCLASS()
class UMGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMGameInstance();
	
	UPROPERTY(BlueprintReadWrite)
	int32 LevelIndex = 1;

	UPROPERTY()
	UAudioComponent* MusicComponent;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* BackgroundMusic;
	
	void PlayMusic();
};
