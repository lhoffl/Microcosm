#include "MEnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MGameplayTags.h"
#include "AbilitySystem/MAbilitySystemComponent.h"
#include "AbilitySystem/MAttributeSet.h"

AMEnemyCharacter::AMEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UMAttributeSet>("AttributeSet");

	bShowName = false;
	bAlwaysShowHealth = false;
}

FGenericTeamId AMEnemyCharacter::GetGenericTeamId() const
{
	return TeamID;
}

void AMEnemyCharacter::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	Super::SetGenericTeamId(InTeamID);
	TeamID = InTeamID;
}

void AMEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(NewController && !NewController->IsPlayerController())
	{
		ServerSideInit();
		InitializeDefaultAttributes();
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AbilitySystemComponent->SetUserAbilityActivationInhibited(false);
	}
}

void AMEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	ClientSideInit();
}

bool AMEnemyCharacter::IsDead() const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(FMGameplayTags::Get().State_Dead);
}

void AMEnemyCharacter::Revive()
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(this, FMGameplayTags::Get().State_Dead.GetSingleTagContainer(), true);	
}