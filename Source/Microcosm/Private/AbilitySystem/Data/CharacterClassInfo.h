#pragma once

class UGameplayAbility;
class UGameplayEffect;
#include "CharacterClassInfo.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8 
{
	None,
	Player,
	Enemy,
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "ClassDefaults")
	TSubclassOf<UGameplayEffect> ClassPrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "ClassDefaults")
	TArray<TSubclassOf<UGameplayAbility>> ClassDefaultAbilities;
};

UCLASS()
class MICROCOSM_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "CharacterClassDefaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterDefaults")
	TSubclassOf<UGameplayEffect> SetAttributesToMaxEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "CommonClassDefaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
	
	// Common to all characters, jump, die etc
	UPROPERTY(EditDefaultsOnly, Category = "CommonClassDefaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "CommonClassDefaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	UPROPERTY(EditDefaultsOnly, Category = "CommonClassDefaults|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;
};