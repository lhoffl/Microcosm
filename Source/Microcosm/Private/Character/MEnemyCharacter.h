#pragma once
#include "MCharacter.h"
#include "AbilitySystem/MAbilityCardActor.h"
#include "MEnemyCharacter.generated.h"

UCLASS()
class AMEnemyCharacter : public AMCharacter
{
	GENERATED_BODY()

public:
	AMEnemyCharacter();
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	void UpdateHandOrder();
	bool IsDead() const;
	void Revive();

	UFUNCTION(BlueprintCallable)
	void DropCurrentCard() const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* EnemyMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Cards")
	TArray<TSubclassOf<AMAbilityCardActor>> AbilityCards;

	UPROPERTY(EditDefaultsOnly, Category = "Cards")
	TArray<UMaterialInterface*> Materials;

	TSubclassOf<AMAbilityCardActor> CurrentCard;
	UPROPERTY()
	UMaterialInterface* CurrentMaterial;
	TDoubleLinkedList<TSubclassOf<AMAbilityCardActor>> AbilityHand;	
	TDoubleLinkedList<UMaterialInterface*> MaterialList;	
	void OnLoopTickIncreased(int CurrentTick);
};
