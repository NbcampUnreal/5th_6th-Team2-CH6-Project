#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PDPawnBase.generated.h"

UCLASS()
class PROJECTD_API APDPawnBase : public APawn
{
	GENERATED_BODY()

public:
	APDPawnBase();

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
