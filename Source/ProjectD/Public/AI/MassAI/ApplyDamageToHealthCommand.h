#pragma once

#include "CoreMinimal.h"
#include "MassCommands.h" 
#include "MassEntityTypes.h"

struct PROJECTD_API FApplyDamageToHealthCommand : public FMassBatchedCommand
{
public:
	FApplyDamageToHealthCommand();
	FApplyDamageToHealthCommand(const FMassEntityHandle InEntity, const float InDamage);

	virtual void Run(FMassEntityManager& EntityManager) override;
	virtual void Reset() override;

private:
	FMassEntityHandle Entity;
	float Damage = 0.f;
};
