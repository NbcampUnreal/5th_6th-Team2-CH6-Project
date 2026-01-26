#pragma once

#include "CoreMinimal.h"
#include "Gimmick/PDOverlapGimmickBase.h"
#include "PDTeleporter.generated.h"

class UCapsuleComponent;

UCLASS()
class PROJECTD_API APDTeleporter : public APDOverlapGimmickBase
{
	GENERATED_BODY()

public:
	APDTeleporter();
	
public:
	// IPDInteractableObject Interface
	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual void OnEndInteract_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gimmick|Component")
	TObjectPtr<UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick|Teleport")
	TObjectPtr<USceneComponent> Destination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick|Teleport")
	TWeakObjectPtr<APDTeleporter> TeleportDest;
};
