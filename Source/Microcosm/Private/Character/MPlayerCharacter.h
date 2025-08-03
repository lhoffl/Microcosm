#pragma once
#include "MCharacter.h"
#include "AbilitySystem/MAbilityCardActor.h"
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

	UFUNCTION()
	void PlayWinMontage() const;
	
protected:
	virtual void InitAbilityActorInfo() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void FellOutOfWorld(const UDamageType& dmgType) override;
	
	virtual void ServerSideInit() override;

	void UpdateHandOrder();

	UFUNCTION()
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

	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	USkeletalMeshComponent* PlayerMesh;
	
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
	
	FAbilityCard LastUsedCard;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* MushroomDeath;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* CardAdded;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* BallLand;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* MushroomDeathAnimation;	

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* MushroomWinAnimation;	
	
	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* MushroomFireballAnimation;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* MushroomIdleAnimation;
	
	int HandSize = 5;
	int CurrentActiveCardIndex = 0;

	void OnActiveAbilityActiveTagChanged(FGameplayTag Tag, int32 NewCount);
};
