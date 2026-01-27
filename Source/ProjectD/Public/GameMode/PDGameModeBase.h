#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PDGameModeBase.generated.h"

UCLASS()
class PROJECTD_API APDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APDGameModeBase();
	
	virtual void BeginPlay() override;
};
