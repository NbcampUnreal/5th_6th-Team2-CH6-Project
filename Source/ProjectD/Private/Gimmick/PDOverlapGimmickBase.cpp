#include "Gimmick/PDOverlapGimmickBase.h"

#include "Components/ShapeComponent.h"

void APDOverlapGimmickBase::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}

	if (Shape.IsValid() && !Shape->OnComponentBeginOverlap.IsAlreadyBound(this, &APDOverlapGimmickBase::OnOverlapGimmick))
	{
		Shape->OnComponentBeginOverlap.AddDynamic(this, &APDOverlapGimmickBase::OnOverlapGimmick);
	}
}

void APDOverlapGimmickBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
	{
		if (Shape.IsValid() && Shape->OnComponentBeginOverlap.IsAlreadyBound(this, &APDOverlapGimmickBase::OnOverlapGimmick))
		{
			Shape->OnComponentBeginOverlap.RemoveDynamic(this, &APDOverlapGimmickBase::OnOverlapGimmick);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void APDOverlapGimmickBase::OnOverlapGimmick(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	Execute_OnInteract(this, OtherActor);
}
