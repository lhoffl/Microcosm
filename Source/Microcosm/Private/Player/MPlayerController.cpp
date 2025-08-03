// copyright lhoffl


#include "Player/MPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "MGameplayTags.h"
#include "AbilitySystem/MAttributeSet.h"
#include "Character/MPlayerCharacter.h"
#include "Input/MInputComponent.h"

AMPlayerController::AMPlayerController()
{

}

void AMPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(MContext);

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MContext, 0);
	}
}

void AMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UMInputComponent* MInputComponent = CastChecked<UMInputComponent>(InputComponent))
	{
		MInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMPlayerController::Move);
		MInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMPlayerController::Look);
		MInputComponent->BindAction(PlayActiveCardAction, ETriggerEvent::Triggered, this, &AMPlayerController::PlayActiveCard);

		MInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

/*void AMPlayerController::SpawnGameplayWidget()
{
	if(!IsLocalPlayerController()) return;

	GameplayWidget = CreateWidget<UGameplayWidget>(this, GameplayWidgetClass);
	if(GameplayWidget)
	{
		GameplayWidget->AddToViewport();
	}
}*/

void AMPlayerController::AcknowledgePossession(APawn* InPawn)
{
	Super::AcknowledgePossession(InPawn);
	if(AMPlayerCharacter* MPlayerCharacter = Cast<AMPlayerCharacter>(InPawn))
	{
		//SpawnGameplayWidget();
	}
}

void AMPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Green, *InputTag.ToString());
	if(GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AMPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Red, *InputTag.ToString());

	if(GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
}

void AMPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Cyan, *InputTag.ToString());
	if(GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
}

// Can't be const, even though Rider thinks it can
void AMPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const UAbilitySystemComponent* ASC = GetASC();
	if(ASC && ASC->HasMatchingGameplayTag(FMGameplayTags::Get().State_Movement_Disable_MovementInputs)) return;
	
	FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	InputAxisVector.Normalize();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const AMPlayerCharacter* MPlayerCharacter = Cast<AMPlayerCharacter>(ControlledPawn);
		if(!MPlayerCharacter) return;

		/*UCharacterMovementComponent* CharacterMovementComponent = MPlayerCharacter->GetCharacterMovement();
		const float MoveSpeed = CharacterMovementComponent->MaxWalkSpeed;*/

		const float MoveSpeed = 100000.f;

		UStaticMeshComponent* BallMesh = MPlayerCharacter->GetBall();
		
		BallMesh->AddForce(MPlayerCharacter->GetMoveForwardDirection() * InputAxisVector.Y * MoveSpeed);
		BallMesh->AddForce(MPlayerCharacter->GetLookRightDirection() * InputAxisVector.X * MoveSpeed);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AMPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(InputAxisVector.X * 0.5f);
		ControlledPawn->AddControllerPitchInput(-InputAxisVector.Y * 0.5f);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AMPlayerController::PlayActiveCard(const FInputActionValue& InputActionValue)
{
	if(AMPlayerCharacter* PlayerCharacter = Cast<AMPlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->PlayActiveCard();
	}
}

void AMPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
}

UMAbilitySystemComponent* AMPlayerController::GetASC()
{
	if(MAbilitySystemComponent == nullptr)
	{
		MAbilitySystemComponent = Cast<UMAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return MAbilitySystemComponent;
}
