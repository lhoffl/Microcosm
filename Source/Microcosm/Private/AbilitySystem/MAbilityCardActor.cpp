// copyright lhoffl


#include "AbilitySystem/MAbilityCardActor.h"

#include "MAbilitySystemComponent.h"
#include "Character/MPlayerCharacter.h"
#include "Components/SphereComponent.h"

void AMAbilityCardActor::OnPickupOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, signed int I, bool bArg, const FHitResult& HitResult)
{
	if(AMPlayerCharacter* PlayerCharacter = Cast<AMPlayerCharacter>(Actor))
	{
		PlayerCharacter->AddCardToHand(AbilityToGrant);
	}

	SetLifeSpan(0.1f);
}

AMAbilityCardActor::AMAbilityCardActor()
{
	PrimaryActorTick.bCanEverTick = true;
	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>("CardMesh");
	CardMesh->SetSimulatePhysics(false);
	CardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(CardMesh);

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
		
}

void AMAbilityCardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

