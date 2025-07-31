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
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//GetCharacterMovement()->bSnapToPlaneAtStart = true;
	//GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>("OverheadStatus");
	OverheadWidget->SetupAttachment(GetRootComponent());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("PerceptionStimuliSourceComponent");

	bShowName = true;
}

void AMCharacter::BeginPlay()
{
	Super::BeginPlay();
	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
}

void AMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ScaleOverheadStatusGauge();
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

void AMCharacter::OnRep_TeamID()
{
}

/*
void AMCharacter::OnMovementModeChanged(const EMovementMode PrevMovementMode, const uint8 PreviousCustomMode)
{
	if(!AbilitySystemComponent) return;
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if(!HasAuthority()) return;
	if (GetCharacterMovement()->IsMovingOnGround() && PrevMovementMode == MOVE_Falling)
	{
		const FGameplayTag GroundedTag = FMGameplayTags::Get().State_Movement_Grounded;
		if(!AbilitySystemComponent->HasMatchingGameplayTag(GroundedTag))
		{
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(AbilitySystemComponent->GetAvatarActor(), GroundedTag.GetSingleTagContainer(), true);
		}
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(AbilitySystemComponent->GetAvatarActor(), FMGameplayTags::Get().State_Movement_InAir.GetSingleTagContainer(), true);
	}
	if (GetCharacterMovement()->IsFalling())
	{
		const FGameplayTag InAirTag = FMGameplayTags::Get().State_Movement_InAir;
		if(!AbilitySystemComponent->HasMatchingGameplayTag(InAirTag))
		{
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(AbilitySystemComponent->GetAvatarActor(), InAirTag.GetSingleTagContainer(), true);
		}
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(AbilitySystemComponent->GetAvatarActor(), FMGameplayTags::Get().State_Movement_Grounded.GetSingleTagContainer(), true);
	}
}*/

bool AMCharacter::IsLocallyControlledByPlayer() const
{
	return GetController() != nullptr && GetController()->IsLocalPlayerController();
}

void AMCharacter::ConfigureOverheadStatusWidget()
{
	if (!OverheadWidget)
	{
		// Retry until widget is constructed
		GetWorldTimerManager().SetTimerForNextTick(this, &AMCharacter::ConfigureOverheadStatusWidget);
		return;
	}
	 
	if(IsLocallyControlledByPlayer())
	{
		OverheadWidget->SetHiddenInGame(true);
		return;
	}

	// Force reinit - fixes nullptr when respawning on clients
	if (!OverheadWidget->GetUserWidgetObject()) OverheadWidget->InitWidget();
	
	OverheadStatusGauge = Cast<UOverheadStatusGauge>(OverheadWidget->GetUserWidgetObject());
	if(OverheadStatusGauge)
	{
		OverheadStatusGauge->ConfigureWithAbilitySystem(AbilitySystemComponent);
		OverheadWidget->SetHiddenInGame(false);
		GetWorldTimerManager().ClearTimer(OverheadStatVisibilityTimerHandle);
		GetWorldTimerManager().SetTimer(OverheadStatVisibilityTimerHandle, this, &AMCharacter::UpdateOverheadStatVisibility, OverheadStatVisibilityTimerPeriod, true);

		if(APawn* LocalPawn = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			OverheadStatusGauge->SetIsEnemyHealthbar(UMAbilitySystemLibrary::IsFriend(this, LocalPawn));
		}
	}
	else
	{
		OverheadWidget->SetHiddenInGame(true);
	}
}

void AMCharacter::ServerSideInit()
{
	InitAbilityActorInfo();
	AddCharacterAbilities();
	AbilitySystemComponent->RegisterGameplayTagEvent(FMGameplayTags::Get().State_FullHealth, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AMCharacter::OnFullHealthTagChanged);
	SetAIPerceptionStimuliSourceEnabled(true);
}

void AMCharacter::OnFullHealthTagChanged(const FGameplayTag GameplayTag, const int32 NewCount) const
{
	if(GameplayTag.MatchesTagExact(FMGameplayTags::Get().State_FullHealth) && NewCount == 0)
	{
		UpdateOverheadStatVisibility();
	}	
}

void AMCharacter::ClientSideInit()
{
	InitAbilityActorInfo();
}

void AMCharacter::UpdateOverheadStatVisibility() const
{
	if(!OverheadStatusGauge) return;
	if(APawn* LocalPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		const float DistanceSquared = FVector::DistSquared(GetActorLocation(), LocalPawn->GetActorLocation());
		const bool bMaxDistanceExceeded = DistanceSquared < OverheadStatVisibilityMaxRangeSquared;
		
		if(UMAbilitySystemLibrary::IsFriend(LocalPawn, this))
		{
			OverheadStatusGauge->SetNameTextVisibility(bMaxDistanceExceeded && bShowName);

			bool bIsFullHealth = false;
			if(AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FMGameplayTags::Get().State_FullHealth))
			{
				bIsFullHealth = true;
			}

			OverheadStatusGauge->SetHealthbarVisibility(!bIsFullHealth || bAlwaysShowHealth);	
			OverheadStatusGauge->SetIsEnemyHealthbar(false);
			return;
		}

		OverheadStatusGauge->SetIsEnemyHealthbar(true);
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), LocalPawn->GetActorLocation(), ECC_Visibility, GetIgnoreCharacterParams());

		bool bIsFullHealth = false;
		if(AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FMGameplayTags::Get().State_FullHealth))
		{
			bIsFullHealth = true;
		}

		OverheadStatusGauge->SetHealthbarVisibility(!HitResult.bBlockingHit && !bIsFullHealth);
		OverheadStatusGauge->SetNameTextVisibility(!HitResult.bBlockingHit && bMaxDistanceExceeded && bShowName);
	}
}

void AMCharacter::Destroyed()
{
	Super::Destroyed();
	if(FullHealthTagChangedDelegate.IsValid()) FullHealthTagChangedDelegate.Reset();
}

void AMCharacter::ScaleOverheadStatusGauge() const
{
	if(!OverheadWidget || !OverheadWidget->IsWidgetVisible()) return;
	if(!OverheadStatusGauge) return;
	if(const APawn* LocalPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		const float Distance = FVector::Dist(LocalPawn->GetActorLocation(), GetActorLocation());
		const float Scale = FMath::GetMappedRangeValueClamped(FVector2D(MinOverheadScale, MaxOverheadScale), FVector2D(1.0f, 0.5f), Distance);
		OverheadStatusGauge->SetRenderScale(FVector2D(Scale, Scale));
	}
}

void AMCharacter::Die()
{
	//Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	//AbilitySystemComponent->TryActivateAbilitiesByTag(FMGameplayTags::Get().State_Dead.GetSingleTagContainer(), true);
	MulticastHandleDeath();
	SetAIPerceptionStimuliSourceEnabled(false);
}


void AMCharacter::MulticastHandleDeath_Implementation()
{
	HandleDeath();
}

void AMCharacter::InitAbilityActorInfo()
{
	/*if(UMAnimInstance* MAnimInstance = Cast<UMAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		if(AbilitySystemComponent) MAnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
	}*/

	ConfigureOverheadStatusWidget();
	bCallbacksBound = false;
	BindCallbacksToDependencies();
	AbilitySystemComponent->RegisterGameplayTagEvent(FMGameplayTags::Get().State_FullHealth, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AMCharacter::OnFullHealthTagChanged);
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

void AMCharacter::HandleDeath()
{
	bIsDead = true;
}

void AMCharacter::SetupHitboxes()
{
	//Hitboxes.Add("main", main);
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

void AMCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMCharacter, TeamID);
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

