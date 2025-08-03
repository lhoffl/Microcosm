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
	void PlaySFX(USoundBase* SFX);

	UPROPERTY(BlueprintReadWrite)
	int32 LevelIndex = 1;

	UPROPERTY()
	UAudioComponent* MusicComponent;

	UPROPERTY()
	UAudioComponent* SFXComponent;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* BackgroundMusic;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, float> LevelNamesToTime;

	UPROPERTY(BlueprintReadOnly)
	float Time = -1.f;
	double StartTime;

	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable)
	float GetElapsedPlayTime() const;

	void PlayMusic();
	
};
