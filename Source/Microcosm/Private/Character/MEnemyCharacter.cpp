#include "MEnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MGameplayTags.h"
#include "MPlayerCharacter.h"
#include "AbilitySystem/MAbilitySystemComponent.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "AbilitySystem/MAttributeSet.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Game/MGameMode.h"
#include "Kismet/GameplayStatics.h"

void AMEnemyCharacter::OnHitBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		if(ASC->HasMatchingGameplayTag(FMGameplayTags::Get().State_Movement_Fireball))
		{
			DropCard();

			EnemyMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			EnemyMesh->SetAnimation(DeathAnimation);
			EnemyMesh->Play(false);

			OnDeathStarted.Broadcast(this);
			SetLifeSpan(10.f);
			EnemyMesh->SetEnableGravity(true);
		}
	}
}

AMEnemyCharacter::AMEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UMAttributeSet>("AttributeSet");

	PhysicsMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Physics");
	PhysicsMesh->SetVisibility(false);
	SetRootComponent(PhysicsMesh);
	
	EnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	EnemyMesh->SetEnableGravity(false);
	EnemyMesh->SetSimulatePhysics(false);
	EnemyMesh->SetupAttachment(GetRootComponent());
	EnemyMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	EnemyMesh->SetCollisionResponseToChannel(ECC_PlayerOnlyOverlap, ECR_Overlap);
	
	HitBox =CreateDefaultSubobject<USphereComponent>("Hitbox");
	HitBox->SetupAttachment(GetRootComponent());
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AMEnemyCharacter::OnHitBoxOverlapped);
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HitBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	HitBox->SetEnableGravity(false);
	HitBox->SetSimulatePhysics(false);
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

	ToggleActive(true);
	EnemyMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	EnemyMesh->SetAnimation(IdleAnimation);
	EnemyMesh->Play(true);
	SetGenericTeamId(1);
}

void AMEnemyCharacter::DropCard()
{
	if(!bIsActive) return;
	if(bCardDropped) return;
	FTransform Transform;
	Transform.SetLocation(GetActorLocation());
	Transform.SetRotation(FQuat::Identity);
	
	AMAbilityCardActor* CardActor = GetWorld()->SpawnActorDeferred<AMAbilityCardActor>(AbilityCard, Transform);
	CardActor->bDestroyAfterGrounded = true;
	CardActor->FinishSpawning(Transform);
	bCardDropped = true;
}

void AMEnemyCharacter::ToggleActive(const bool bActive)
{
	bIsActive = bActive;
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
	HitBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	if(bActive)
	{
		EnemyMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		EnemyMesh->SetCollisionResponseToChannel(ECC_PlayerOnlyOverlap, ECR_Overlap);	
	}
	else
	{
		EnemyMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		EnemyMesh->SetCollisionResponseToChannel(ECC_PlayerOnlyOverlap, ECR_Ignore);		
	}
}


