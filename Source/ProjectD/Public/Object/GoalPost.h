#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick/PDInteractableObject.h"
#include "GoalPost.generated.h"

class ABallCore;
class APawn;
class UStaticMeshComponent;

UCLASS()
class PROJECTD_API AGoalPost : public AActor, public IPDInteractableObject
{
	GENERATED_BODY()

public:
	AGoalPost();

	virtual void OnInteract_Implementation(AActor* Interactor) override;

	bool CanPlaceBall(APawn* Pawn, ABallCore* Ball) const;

	void Server_PlaceBall(APawn* Pawn, ABallCore* Ball);
	void Server_StealBall(APawn* Stealer);

protected:
	UPROPERTY()
	TObjectPtr<ABallCore> PlacedBall = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	FTimerHandle HoldTimer;

	void StartHoldTimer();
	void OnHoldComplete();
};
