// copyright lhoffl


#include "Game/MGameInstance.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

UMGameInstance::UMGameInstance()
{
	MusicComponent = CreateDefaultSubobject<UAudioComponent>("Music");
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
