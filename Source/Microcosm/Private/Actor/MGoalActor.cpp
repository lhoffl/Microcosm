// copyright lhoffl


#include "Actor/MGoalActor.h"

#include "Character/MPlayerCharacter.h"

AMGoalActor::AMGoalActor()
{
	PrimaryActorTick.bCanEverTick = true;

	GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>("GoalMesh");
	SetRootComponent(GoalMesh);
	
	GoalBox = CreateDefaultSubobject<UBoxComponent>("GoalBox");
	GoalBox->SetupAttachment(GetRootComponent());
	GoalBox->SetGenerateOverlapEvents(true);
	GoalBox->OnComponentBeginOverlap.AddDynamic(this, &AMGoalActor::OnGoalOverlap);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>("PointLight");
	PointLightComponent->SetupAttachment(GetRootComponent());	

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>("SpotLight");
	SpotLightComponent->SetupAttachment(GetRootComponent());

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingComp");
}

void AMGoalActor::BeginPlay()
{
	Super::BeginPlay();
	RotatingMovementComponent->RotationRate = RotationRate;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AMGoalActor::OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor) return;
	if(!Cast<AMPlayerCharacter>(OtherActor)) return;

	GoalBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnGoalReached.Broadcast(GoalType);
}

void AMGoalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

