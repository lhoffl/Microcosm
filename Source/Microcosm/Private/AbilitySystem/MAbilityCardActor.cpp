// copyright lhoffl


#include "AbilitySystem/MAbilityCardActor.h"

#include "Character/MPlayerCharacter.h"
#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"

void AMAbilityCardActor::OnPickupOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, signed int I, bool bArg, const FHitResult& HitResult)
{
	if(AMPlayerCharacter* PlayerCharacter = Cast<AMPlayerCharacter>(Actor))
	{
		if(PlayerCharacter->AddCardToHand(AbilityCard)) Destroy();
	}
}

AMAbilityCardActor::AMAbilityCardActor()
{
	PrimaryActorTick.bCanEverTick = true;
	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>("CardMesh");
	CardMesh->SetSimulatePhysics(false);
	CardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CardMesh->SetVisibility(false);
	SetRootComponent(CardMesh);

	CardBillboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	CardBillboard->SetupAttachment(GetRootComponent());
	CardBillboard->SetHiddenInGame(false);
	CardBillboard->Sprite = Texture2D;
	
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovement");
	RotatingMovementComponent->RotationRate = RotationRate;
	
	PickupSphere = CreateDefaultSubobject<USphereComponent>("PickupSphere");
	PickupSphere->SetupAttachment(GetRootComponent());
	PickupSphere->SetGenerateOverlapEvents(true);
	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AMAbilityCardActor::OnPickupOverlap);
}

void AMAbilityCardActor::BeginPlay()
{
	Super::BeginPlay();
	AbilityCard = FAbilityCard(AbilityToGrant, Texture2D, Color);
	CardBillboard->Sprite = Texture2D;
	
	if(bDestroyAfterGrounded) GetWorldTimerManager().SetTimer(FallTimerHandle, this, &AMAbilityCardActor::OnFallTimerExpired, TimeBeforeFalling);
}

void AMAbilityCardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!bIsGrounded && bDestroyAfterGrounded && bFallTimerExpired)
	{
		FHitResult HitResult;
		FVector Start = GetActorLocation();
		FVector End = Start - FVector(0.f, 0.f, 100.f);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

		if(HitResult.bBlockingHit)
		{
			bIsGrounded = true;
			CardMesh->SetSimulatePhysics(false);
			CardMesh->SetMassScale(NAME_None, 0.f);
			CardMesh->SetEnableGravity(false);
		}
		else
		{
			CardMesh->SetSimulatePhysics(true);
			CardMesh->SetMassScale(NAME_None, 1.f);
			CardMesh->SetEnableGravity(true);	
			if(bDestroyAfterGrounded && bFallTimerExpired) SetLifeSpan(DestroyDelayAfterGrounded);
		}	
	}
}

void AMAbilityCardActor::OnFallTimerExpired()
{
	CardMesh->SetSimulatePhysics(true);
	CardMesh->SetMassScale(NAME_None, 1.f);
	CardMesh->SetEnableGravity(true);
	bFallTimerExpired = true;
}
