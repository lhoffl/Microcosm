// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MAssetManager.generated.h"

UCLASS()
class MICROCOSM_API UMAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:

	static UMAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
