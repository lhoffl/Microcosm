// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "MGoalActor.generated.h"

UENUM(BlueprintType)
enum class EGoalType : uint8
{
	Standard,
	Skip
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalReached, EGoalType, GoalType);

UCLASS()
class AMGoalActor : public AActor
{
	GENERATED_BODY()
	
public:
	AMGoalActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintCallable)
	FOnGoalReached OnGoalReached;

	UPROPERTY(EditDefaultsOnly)
	EGoalType GoalType = EGoalType::Standard;

	UPROPERTY(EditAnywhere)
	FRotator RotationRate = FRotator::ZeroRotator;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* GoalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* GoalMesh;

	UPROPERTY(EditDefaultsOnly)
	UPointLightComponent* PointLightComponent;

	UPROPERTY(EditDefaultsOnly)
	USpotLightComponent* SpotLightComponent;

	UPROPERTY(EditDefaultsOnly)
	URotatingMovementComponent* RotatingMovementComponent;
	
	UFUNCTION()
	void OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
