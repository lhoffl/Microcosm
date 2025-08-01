// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "MAbilityCardActor.generated.h"

class USphereComponent;

USTRUCT(BlueprintType)
struct FAbilityCard
{
	GENERATED_BODY()

	TSubclassOf<UGameplayAbility> AbilityToGrant;

	UPROPERTY(BlueprintReadOnly)
	FLinearColor Color = FLinearColor::Black;
	
	UPROPERTY()
	UMaterialInterface* Material = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsActiveCard = false;

	explicit FAbilityCard(const TSubclassOf<UGameplayAbility>& InAbility, UMaterialInterface* InMaterial, const FLinearColor InColor)
	{
		AbilityToGrant = InAbility;
		Material = InMaterial;
		Color = InColor;
	}

	FAbilityCard() {}

	bool operator==(const FAbilityCard& Other) const
	{
		return AbilityToGrant == Other.AbilityToGrant;
	}
};

UCLASS()
class AMAbilityCardActor : public AActor
{
	GENERATED_BODY()
	
public:
	AMAbilityCardActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	bool bDestroyAfterGrounded = false;

protected:
	void OnFallTimerExpired();
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly)
	FAbilityCard AbilityCard;
	FTimerHandle FallTimerHandle;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Card|Ability")
	TSubclassOf<UGameplayAbility> AbilityToGrant;

	UPROPERTY(EditDefaultsOnly, Category = "Card|Ability")
	FLinearColor Color;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* PickupSphere;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CardMesh;

	UPROPERTY(EditDefaultsOnly)
	float TimeBeforeFalling = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float DestroyDelayAfterGrounded = 10.f;

	bool bIsGrounded = false;
	bool bFallTimerExpired = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Card|Rotation")
	FRotator RotationRate = FRotator(0.f, 100.f, 0.f); 
	
	UFUNCTION()
	void OnPickupOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, signed int I, bool bArg, const FHitResult& HitResult);

	UPROPERTY(EditDefaultsOnly)
	URotatingMovementComponent* RotatingMovementComponent;
};
