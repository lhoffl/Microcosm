#pragma once
#include "MCharacter.h"
#include "AbilitySystem/MAbilityCardActor.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathStarted, AMEnemyCharacter*, EnemyCharacter);

UCLASS()
class AMEnemyCharacter : public AMCharacter
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnHitBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	AMEnemyCharacter();
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void DropCard();
 
	FOnDeathStarted OnDeathStarted;

	UFUNCTION()
	void ToggleActive(const bool bActive);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* EnemyMesh;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* PhysicsMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Cards")
	TSubclassOf<AMAbilityCardActor> AbilityCard;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* HitBox;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* DeathAnimation;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* IdleAnimation;
	
	bool bIsActive = false;
	bool bCardDropped = false;
};
