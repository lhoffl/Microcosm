// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "MGoalActor.generated.h"

UENUM()
enum EGoalType
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

	EGoalType GoalType = EGoalType::Standard;
	
	UFUNCTION()
	void OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
