#include "MEnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MGameplayTags.h"
#include "AbilitySystem/MAbilitySystemComponent.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "AbilitySystem/MAttributeSet.h"
#include "Game/MGameMode.h"
#include "Kismet/GameplayStatics.h"

AMEnemyCharacter::AMEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UMAttributeSet>("AttributeSet");

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	EnemyMesh->SetSimulatePhysics(true);
	SetRootComponent(EnemyMesh);	
	
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

	if (AMGameMode* GM = Cast<AMGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnLoopTickIncreased.AddUObject(this, &AMEnemyCharacter::OnLoopTickIncreased);
	}

	for(TSubclassOf<AMAbilityCardActor> Card : AbilityCards)
	{
		AbilityHand.AddHead(Card);
	}

	for(UMaterialInterface* Material : Materials)
	{
		MaterialList.AddHead(Material);
	}
	
	if(AbilityHand.Num() > 0)
	{
		CurrentCard = AbilityHand.GetHead()->GetValue();
		CurrentMaterial = MaterialList.GetHead()->GetValue();
	}
}

void AMEnemyCharacter::UpdateHandOrder()
{
	if(AbilityHand.Num() <= 1) return;

	if (TDoubleLinkedList<TSubclassOf<AMAbilityCardActor>>::TDoubleLinkedListNode* Tail = AbilityHand.GetTail())
	{
		const TSubclassOf<AMAbilityCardActor> TailValue = Tail->GetValue();
		AbilityHand.RemoveNode(Tail);
		AbilityHand.AddHead(TailValue);
		CurrentCard = TailValue;
	}

	if (TDoubleLinkedList<UMaterialInterface*>::TDoubleLinkedListNode* Tail = MaterialList.GetTail())
	{
		UMaterialInterface* TailValue = Tail->GetValue();
		MaterialList.RemoveNode(Tail);
		MaterialList.AddHead(TailValue);
		CurrentMaterial = TailValue;
		EnemyMesh->SetMaterial(0, CurrentMaterial);
		EnemyMesh->SetMaterial(1, CurrentMaterial);
	}
}

bool AMEnemyCharacter::IsDead() const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(FMGameplayTags::Get().State_Dead);
}

void AMEnemyCharacter::Revive()
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(this, FMGameplayTags::Get().State_Dead.GetSingleTagContainer(), true);	
}

void AMEnemyCharacter::DropCurrentCard() const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AMAbilityCardActor>(CurrentCard, GetActorLocation(), FQuat::Identity.Rotator(), SpawnParameters);
}

void AMEnemyCharacter::OnLoopTickIncreased(int CurrentTick)
{
	UpdateHandOrder();
}
