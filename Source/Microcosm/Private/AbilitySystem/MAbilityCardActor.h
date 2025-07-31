// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "MAbilityCardActor.generated.h"

class USphereComponent;

UCLASS()
class AMAbilityCardActor : public AActor
{
	GENERATED_BODY()
	
public:
	AMAbilityCardActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Card|Ability")
	TSubclassOf<UGameplayAbility> AbilityToGrant;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* PickupSphere;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CardMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Card|Rotation")
	FRotator RotationRate = FRotator(0.f, 100.f, 0.f); 
	
	UFUNCTION()
	void OnPickupOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, signed int I, bool bArg, const FHitResult& HitResult);

	UPROPERTY(EditDefaultsOnly)
	URotatingMovementComponent* RotatingMovementComponent;
};
