#pragma once
#include "MCharacter.h"
#include "AbilitySystem/MAbilityCardActor.h"
#include "AbilitySystem/Abilities/MCardAbility.h"
#include "MPlayerCharacter.generated.h"

class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHandUpdated);

UCLASS()
class AMPlayerCharacter : public AMCharacter
{
	GENERATED_BODY()
	
public:
	AMPlayerCharacter();

	UFUNCTION(BlueprintCallable)
	FVector GetLookRightDirection() const;

	UFUNCTION(BlueprintCallable)
	FVector GetLookForwardDirection() const;

	UFUNCTION(BlueprintCallable)
	FVector GetMoveForwardDirection() const;

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetBall() const; 

	UFUNCTION(BlueprintCallable)
	bool AddCardToHand(const FAbilityCard& AbilityCard);

	UFUNCTION(BlueprintCallable)
	void PlayActiveCard();

	UPROPERTY(BlueprintAssignable)
	FOnHandUpdated OnHandUpdated;

	UFUNCTION(BlueprintCallable)
	const TArray<FAbilityCard> GetHand() const;
	
protected:
	virtual void InitAbilityActorInfo() override;
	void RespawnTimerFinished();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void ServerSideInit() override;

	virtual void Die() override;

	void UpdateHandOrder();

	void OnLoopTickIncreased(int CurrentTick);
	virtual void BeginPlay() override;

	void UpdateFireballTag();
	virtual void Tick(float DeltaSeconds) override;
	
private:
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Character|View")
	class USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Character|View")
	class UCameraComponent* ThirdPersonCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	UStaticMeshComponent* BallMesh;
	
	UPROPERTY(EditDefaultsOnly)
	float RespawnTimeDelay = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float FireballVelocity = 2000.f;
	
	UPROPERTY(EditDefaultsOnly)
	float FireballVelocityGracePeriod = 1.f;

	bool bFireballVelocityAchievedThisFrame = false;
	
	FTimerHandle FireballGracePeriodTimer;
	
	FTimerHandle RespawnTimer;

	bool bPlayedActiveCardThisLoop = false;
	
	TDoubleLinkedList<FAbilityCard> AbilityHand;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> DefaultAbility;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor DefaultAbilityColor;
	
	FAbilityCard DefaultCard;
	FAbilityCard LastUsedCard;
	
	int HandSize = 5;
	int CurrentActiveCardIndex = 0;

	void OnActiveAbilityActiveTagChanged(FGameplayTag Tag, int32 NewCount);
};
