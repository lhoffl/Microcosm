#include "MPlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MGameplayTags.h"
#include "AbilitySystem/MAbilityCardActor.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Game/MGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MPlayerState.h"

AMPlayerCharacter::AMPlayerCharacter()
{
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ball");
	BallMesh->SetSimulatePhysics(true);
	SetRootComponent(BallMesh);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>("3rd person camera");
	ThirdPersonCamera->SetupAttachment(SpringArmComponent);

	bUseControllerRotationYaw = false;

	SetNetUpdateFrequency(100.f);
	SetMinNetUpdateFrequency(50.f);
}

FVector AMPlayerCharacter::GetLookRightDirection() const
{
	return ThirdPersonCamera->GetRightVector();
}

FVector AMPlayerCharacter::GetLookForwardDirection() const
{
	return ThirdPersonCamera->GetForwardVector();
}

FVector AMPlayerCharacter::GetMoveForwardDirection() const
{
	return FVector::CrossProduct(GetLookRightDirection(), FVector::UpVector);
}

UStaticMeshComponent* AMPlayerCharacter::GetBall() const
{
	return BallMesh;
}

bool AMPlayerCharacter::AddCardToHand(const FAbilityCard& AbilityCard)
{
	if(AbilityHand.Num() >= HandSize) return false;
	if(AbilityHand.Contains(DefaultCard))
	{
		if(AbilityCard == DefaultCard) return false;
		if(AbilityCard != DefaultCard) AbilityHand.RemoveNode(DefaultCard);
	}
	
	if(AbilityCard == DefaultCard && LastUsedCard == DefaultCard) return false;
	
	AbilityHand.AddTail(AbilityCard);
	OnHandUpdated.Broadcast();
	return true;
}

void AMPlayerCharacter::PlayActiveCard()
{
	if(bPlayedActiveCardThisLoop) return;
	if(AbilityHand.Num() <= 0) return;

	TDoubleLinkedList<FAbilityCard>::TDoubleLinkedListNode* Head = AbilityHand.GetHead();
	if(!Head) return;
	
	const FAbilityCard& ActiveAbilityCard = Head->GetValue();
	const TSubclassOf<UGameplayAbility> CardAbility = ActiveAbilityCard.AbilityToGrant;
	if(!CardAbility) return;
	
	bPlayedActiveCardThisLoop = true;
	LastUsedCard = ActiveAbilityCard;

	bool bWasSuccessful = false;
	GetMAbilitySystemComponent()->AddAbility(CardAbility);
	FGameplayAbilitySpecHandle AbilityHandle = UMAbilitySystemLibrary::TryActivateAbilityFromSpec(GetMAbilitySystemComponent(), CardAbility, bWasSuccessful);

	if(bWasSuccessful)
	{
		OnHandUpdated.Broadcast();
		AbilityHand.RemoveNode(Head);
	}
	else
	{
		OnHandUpdated.Broadcast();
		GetMAbilitySystemComponent()->RemoveLooseGameplayTag(FMGameplayTags::Get().Abilities_ActiveCard_Active, 1);
		GetMAbilitySystemComponent()->ClearAbility(AbilityHandle);
	}
}

const TArray<FAbilityCard> AMPlayerCharacter::GetHand() const
{
	TArray<FAbilityCard> Result;
	for (auto Node = AbilityHand.GetHead(); Node; Node = Node->GetNextNode())
	{
		Result.Add(Node->GetValue());
	}
	return Result;
}

void AMPlayerCharacter::InitAbilityActorInfo()
{
	AMPlayerState* MPlayerState = GetPlayerState<AMPlayerState>();
	check(MPlayerState);

	MPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MPlayerState, this);
	AbilitySystemComponent = MPlayerState->GetAbilitySystemComponent();
	AttributeSet = MPlayerState->GetAttributeSet();

	if(HasAuthority())
	{
		InitializeDefaultAttributes();
		AddCharacterAbilities();
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(this, FMGameplayTags::Get().State_Dead.GetSingleTagContainer(), true);
	}

	AbilitySystemComponent->SetUserAbilityActivationInhibited(false);
	
	Super::InitAbilityActorInfo();
}

void AMPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ServerSideInit();
	if(const IGenericTeamAgentInterface* PlayerStateTeamAgent = Cast<IGenericTeamAgentInterface>(GetPlayerState()))
	{
		SetGenericTeamId(PlayerStateTeamAgent->GetGenericTeamId());
	}
}

void AMPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	ClientSideInit();
	
	if(const IGenericTeamAgentInterface* PlayerStateTeamAgent = Cast<IGenericTeamAgentInterface>(GetPlayerState()))
	{
		SetGenericTeamId(PlayerStateTeamAgent->GetGenericTeamId());
	}
}

void AMPlayerCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	Super::FellOutOfWorld(dmgType);
	if(AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
	{
		if(const AMGameMode* MGameMode = Cast<AMGameMode>(GameMode))
		{
			MGameMode->RestartLevel();	
		}
	}
}

void AMPlayerCharacter::ServerSideInit()
{
	Super::ServerSideInit();
}

void AMPlayerCharacter::UpdateHandOrder()
{
	if(AbilityHand.Num() <= 1) return;

	if (TDoubleLinkedList<FAbilityCard>::TDoubleLinkedListNode* Tail = AbilityHand.GetTail())
	{
		FAbilityCard TailValue = Tail->GetValue();
		TailValue.bIsActiveCard = true;
		AbilityHand.RemoveNode(Tail);
		AbilityHand.AddHead(TailValue);
	}

	OnHandUpdated.Broadcast();
}

void AMPlayerCharacter::OnLoopTickIncreased(int CurrentTick)
{
	bPlayedActiveCardThisLoop = false;
	UpdateHandOrder();
}

void AMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AMGameMode* GM = Cast<AMGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnLoopTickIncreased.AddDynamic(this, &AMPlayerCharacter::OnLoopTickIncreased);
	}

	DefaultCard = FAbilityCard(DefaultAbility, nullptr, DefaultAbilityColor);
	LastUsedCard = FAbilityCard();
	AddCardToHand(DefaultCard);
	OnHandUpdated.Broadcast();
	
	if(UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->RegisterGameplayTagEvent(FMGameplayTags::Get().Abilities_ActiveCard_Active, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AMPlayerCharacter::OnActiveAbilityActiveTagChanged);
	}
}

void AMPlayerCharacter::UpdateFireballTag()
{
	if(bFireballVelocityAchievedThisFrame) return;
	if(const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if(ASC->HasMatchingGameplayTag(FMGameplayTags::Get().State_Movement_Fireball))
		{
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(this, FMGameplayTags::Get().State_Movement_Fireball.GetSingleTagContainer(), true);
		}
	}		
}

void AMPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 100.f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if(HitResult.bBlockingHit)
	{
		if(const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		{
			if(!ASC->HasMatchingGameplayTag(FMGameplayTags::Get().State_Movement_Grounded))
			{
				UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(this, FMGameplayTags::Get().State_Movement_Grounded.GetSingleTagContainer(), true);
			}
		}
	}
	else
	{
		if(const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		{
			if(ASC->HasMatchingGameplayTag(FMGameplayTags::Get().State_Movement_Grounded))
			{
				UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(this, FMGameplayTags::Get().State_Movement_Grounded.GetSingleTagContainer(), true);
			}
		}	
	}

	if(BallMesh->GetComponentVelocity().Size() > FireballVelocity)
	{
		if(const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		{
			if(!ASC->HasMatchingGameplayTag(FMGameplayTags::Get().State_Movement_Fireball))
			{
				UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(this, FMGameplayTags::Get().State_Movement_Fireball.GetSingleTagContainer(), true);
				bFireballVelocityAchievedThisFrame = true;
			}
		}
	}
	else
	{
		if(!GetWorldTimerManager().IsTimerActive(FireballGracePeriodTimer))
		{
			GetWorldTimerManager().SetTimer(FireballGracePeriodTimer, this, &AMPlayerCharacter::UpdateFireballTag, FireballVelocityGracePeriod);
		}
		bFireballVelocityAchievedThisFrame = false;
	}
}

void AMPlayerCharacter::OnActiveAbilityActiveTagChanged(const FGameplayTag Tag, const int32 NewCount)
{
	if(Tag.MatchesTagExact(FMGameplayTags::Get().Abilities_ActiveCard_Active))
	{
		if(NewCount != 0) return;
		if(AbilityHand.IsEmpty()) AddCardToHand(DefaultCard);
		OnHandUpdated.Broadcast();
	}
}