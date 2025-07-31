// copyright lhoffl


#include "MAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "MGameplayTags.h"
#include "Engine/AssetManager.h"

UMAssetManager& UMAssetManager::Get()
{
	check(GEngine);
	UMAssetManager* MAssetManager = Cast<UMAssetManager>(GEngine->AssetManager);
	return *MAssetManager;
}

void UMAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FMGameplayTags::InitializeNativeGameplayTags();

	// Required to use Target Data
	UAbilitySystemGlobals::Get().InitGlobalData();
}
