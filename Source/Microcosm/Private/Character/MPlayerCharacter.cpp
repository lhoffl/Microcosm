#include "MPlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "Game/MGameMode.h"
#include "GameFramework/SpringArmComponent.h"
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
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

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

void AMPlayerCharacter::AddCardToHand(const TSubclassOf<UGameplayAbility>& CardAbility)
{
	AbilityHand.AddHead(CardAbility);
}

void AMPlayerCharacter::PlayActiveCard()
{
	if(AbilityHand.Num() <= 0) return;
	const TSubclassOf<UGameplayAbility> ActiveAbility = AbilityHand.GetHead()->GetValue();
	if(!ActiveAbility) return;
	
	AbilityHand.RemoveNode(ActiveAbility);
	GetMAbilitySystemComponent()->AddAbility(ActiveAbility);
	GetMAbilitySystemComponent()->TryActivateAbilityByClass(ActiveAbility);
}

void AMPlayerCharacter::InitAbilityActorInfo()
{
	AMPlayerState* MPlayerState = GetPlayerState<AMPlayerState>();
	check(MPlayerState);

	bIsDead = false;
	
	MPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MPlayerState, this);
	AbilitySystemComponent = MPlayerState->GetAbilitySystemComponent();
	AttributeSet = MPlayerState->GetAttributeSet();

	if(HasAuthority())
	{
		InitializeDefaultAttributes();
		AddCharacterAbilities();
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(this, FMGameplayTags::Get().State_Dead.GetSingleTagContainer(), true);
	}

	//ConfigureOverheadStatusWidget();
	
	//GetCharacterMovement()->Activate();

	AbilitySystemComponent->SetUserAbilityActivationInhibited(false);
	
	//FGameplayAbilitySpecHandle CurrentWeaponAbilityHandle = GetMAbilitySystemComponent()->AddAbility(WeaponAbility);
	
	Super::InitAbilityActorInfo();
}

void AMPlayerCharacter::RespawnTimerFinished()
{
	if(AMGameMode* MGameMode = GetWorld()->GetAuthGameMode<AMGameMode>())
	{
		//MGameMode->RequestRespawn(this, Controller);
	}
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

void AMPlayerCharacter::ServerSideInit()
{
	Super::ServerSideInit();
}

void AMPlayerCharacter::Die()
{
	//GetWorldTimerManager().SetTimer(RespawnTimer, this, &AMPlayerCharacter::RespawnTimerFinished, RespawnTimeDelay);
	//Super::Die();
}