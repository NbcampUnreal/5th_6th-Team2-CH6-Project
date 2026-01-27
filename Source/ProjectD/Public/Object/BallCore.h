#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick/PDInteractableObject.h"
#include "BallCore.generated.h"

class APawn;
class UStaticMeshComponent;

UCLASS()
class PROJECTD_API ABallCore : public AActor, public IPDInteractableObject
{
	GENERATED_BODY()

public:
	ABallCore();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnInteract_Implementation(AActor* Interactor) override;

	// === 상태 ===
	UPROPERTY(ReplicatedUsing=OnRep_CarrierPawn, BlueprintReadOnly, VisibleInstanceOnly, Category="Ball")
	TObjectPtr<APawn> CarrierPawn = nullptr;

	// === 서버 API ===
	void Server_SetCarrier(APawn* NewCarrier);
	void Server_ClearCarrier();
	void Server_DropPhysics(const FVector& DropLocation, const FVector& Impulse);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UFUNCTION()
	void OnRep_CarrierPawn();

	void HandleCarrierChanged();
};
