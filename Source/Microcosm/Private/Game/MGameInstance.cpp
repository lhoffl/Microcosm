// copyright lhoffl


#include "Game/MGameInstance.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

UMGameInstance::UMGameInstance()
{
	MusicComponent = CreateDefaultSubobject<UAudioComponent>("Music");
	SFXComponent = CreateDefaultSubobject<UAudioComponent>("SFX");
}

void UMGameInstance::PlaySFX(USoundBase* SFX)
{
	SFXComponent = UGameplayStatics::CreateSound2D(this, SFX, 1, 1, 1, nullptr, true);
	SFXComponent->Play();
}

void UMGameInstance::PlayMusic()
{
	if (BackgroundMusic)
	{
		if(MusicComponent && MusicComponent->IsPlaying()) return;
		
		MusicComponent = UGameplayStatics::CreateSound2D(this, BackgroundMusic, 1, 1, 1, nullptr, true);
		if (MusicComponent)
		{
			MusicComponent->bIsMusic = true;
			MusicComponent->bAutoActivate = false;
			MusicComponent->Play();
		}
	}
}

void UMGameInstance::Init()
{
	Super::Init();
	StartTime = FPlatformTime::Seconds();
}

void UMGameInstance::Shutdown()
{
	Super::Shutdown();
}

float UMGameInstance::GetElapsedPlayTime() const
{
	return static_cast<float>(FPlatformTime::Seconds() - StartTime);
}