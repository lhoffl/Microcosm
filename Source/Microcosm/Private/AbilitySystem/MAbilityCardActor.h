// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/BillboardComponent.h"
#include "MAbilityCardActor.generated.h"

class USphereComponent;

USTRUCT(BlueprintType)
struct FAbilityCard
{
	GENERATED_BODY()

	TSubclassOf<UGameplayAbility> AbilityToGrant;

	UPROPERTY(BlueprintReadOnly)
	FLinearColor Color = FLinearColor::Black;
	
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Texture2D = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsActiveCard = false;

	explicit FAbilityCard(const TSubclassOf<UGameplayAbility>& InAbility, UTexture2D* InTexture2D, const FLinearColor InColor)
	{
		AbilityToGrant = InAbility;
		Texture2D = InTexture2D;
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
	UTexture2D* Texture2D;

	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* CardBillboard;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CardMesh;

	UPROPERTY(EditDefaultsOnly)
	float TimeBeforeFalling = 2.f;

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
