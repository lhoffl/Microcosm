// copyright lhoffl


#include "Character/MCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MGameplayTags.h"
#include "AbilitySystem/MAbilitySystemComponent.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "AbilitySystem/MAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "UI/Widgets/OverheadStatusGauge.h"


AMCharacter::AMCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>("OverheadStatus");
	OverheadWidget->SetupAttachment(GetRootComponent());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("PerceptionStimuliSourceComponent");
}

void AMCharacter::BeginPlay()
{
	Super::BeginPlay();
	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
}

void AMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMCharacter::EliminationSecured()
{
}

FCollisionQueryParams AMCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

FGenericTeamId AMCharacter::GetGenericTeamId() const
{
	return TeamID;
}

void AMCharacter::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	IGenericTeamAgentInterface::SetGenericTeamId(InTeamID);
	TeamID = InTeamID;
}

void AMCharacter::SetAIPerceptionStimuliSourceEnabled(const bool bIsEnabled) const
{
	if(!PerceptionStimuliSourceComponent) return;

	if(bIsEnabled) PerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
	else PerceptionStimuliSourceComponent->UnregisterFromPerceptionSystem();
}

bool AMCharacter::IsLocallyControlledByPlayer() const
{
	return GetController() != nullptr && GetController()->IsLocalPlayerController();
}

void AMCharacter::ServerSideInit()
{
	InitAbilityActorInfo();
	AddCharacterAbilities();
	SetAIPerceptionStimuliSourceEnabled(true);
}

void AMCharacter::Destroyed()
{
	Super::Destroyed();
	if(FullHealthTagChangedDelegate.IsValid()) FullHealthTagChangedDelegate.Reset();
}

void AMCharacter::InitAbilityActorInfo()
{
	bCallbacksBound = false;
	BindCallbacksToDependencies();
}

void AMCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AMCharacter::InitializeDefaultAttributes() const
{
	if(UCharacterClassInfo* CharacterClassInfo = UMAbilitySystemLibrary::GetCharacterClassInfo(GetOwner()))
	{
		const TSubclassOf<UGameplayEffect> VitalAttributesEffect = CharacterClassInfo->VitalAttributes;
		const TSubclassOf<UGameplayEffect> ClassAttributesEffect = CharacterClassInfo->GetClassDefaultInfo(CharacterClass).ClassPrimaryAttributes;
		
		FGameplayEffectQuery EffectQuery = FGameplayEffectQuery();
		EffectQuery.EffectDefinition = VitalAttributesEffect;
		AbilitySystemComponent->RemoveActiveEffects(EffectQuery);

		EffectQuery.EffectDefinition = ClassAttributesEffect;
		AbilitySystemComponent->RemoveActiveEffects(EffectQuery);

		ApplyEffectToSelf(VitalAttributesEffect, 1.f);
		ApplyEffectToSelf(ClassAttributesEffect, 1.f);
		ApplyEffectToSelf(CharacterClassInfo->SetAttributesToMaxEffect, 1.f);

		if(!AbilitySystemComponent->HasMatchingGameplayTag(FMGameplayTags::Get().State_Movement_Grounded))
		{
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(AbilitySystemComponent->GetAvatarActor(), FMGameplayTags::Get().State_Movement_Grounded.GetSingleTagContainer(), true);
		}
	}
}

void AMCharacter::AddCharacterAbilities() const
{
	if(!HasAuthority()) return;

	UMAbilitySystemComponent* MASC = CastChecked<UMAbilitySystemComponent>(AbilitySystemComponent);
	
	if(UCharacterClassInfo* CharacterClassInfo = UMAbilitySystemLibrary::GetCharacterClassInfo(this))
	{
		TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAdd;
		TArray<TSubclassOf<UGameplayAbility>> CommonAbilities = CharacterClassInfo->CommonAbilities;
		TArray<TSubclassOf<UGameplayAbility>> ClassAbilities = CharacterClassInfo->GetClassDefaultInfo(CharacterClass).ClassDefaultAbilities;

		for(TSubclassOf<UGameplayAbility> Ability : CommonAbilities)
		{
			if(Ability) AbilitiesToAdd.AddUnique(Ability);
		}

		for(TSubclassOf<UGameplayAbility> Ability : ClassAbilities)
		{
			if(Ability) AbilitiesToAdd.AddUnique(Ability);
		}

		MASC->AddCharacterAbilities(AbilitiesToAdd);
	}
}

void AMCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(NewController && !NewController->IsPlayerController())
	{
		ServerSideInit();
	}
}

void AMCharacter::BindCallbacksToDependencies()
{
	if(!bCallbacksBound && IsValid(AbilitySystemComponent) && IsValid(AttributeSet))
	{
		bCallbacksBound = true;

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMoveSpeedAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				//GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
			}
			);
	}
	else
	{
		// ensure callbacks get bound 
		GetWorldTimerManager().SetTimerForNextTick(this, &AMCharacter::BindCallbacksToDependencies);
	}
}

