#pragma once
#include "MCharacter.h"
#include "MPlayerCharacter.generated.h"

class USphereComponent;

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

	UStaticMeshComponent* GetBall() const; 

	void AddCardToHand(const TSubclassOf<UGameplayAbility>& CardAbility);
	void PlayActiveCard();
	
protected:
	virtual void InitAbilityActorInfo() override;
	void RespawnTimerFinished();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void ServerSideInit() override;

	virtual void Die() override;

private:
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Character|View")
	class USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Character|View")
	class UCameraComponent* ThirdPersonCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	UStaticMeshComponent* BallMesh;
	
	UPROPERTY(EditDefaultsOnly)
	float RespawnTimeDelay = 5.f;
	
	FTimerHandle RespawnTimer;

	TDoubleLinkedList<TSubclassOf<UGameplayAbility>> AbilityHand;
};
