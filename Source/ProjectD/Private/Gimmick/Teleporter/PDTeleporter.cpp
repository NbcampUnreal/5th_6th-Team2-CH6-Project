#include "Gimmick/Teleporter/PDTeleporter.h"

#include "Components/CapsuleComponent.h"

#include "MoverComponent.h"

APDTeleporter::APDTeleporter()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CapsuleComp->SetupAttachment(RootComponent);

	Shape = CapsuleComp;
}

void APDTeleporter::OnInteract_Implementation(AActor* Interactor)
{
	if (!TeleportDest.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("APDTeleporter::OnInteract - Invalid Destination!"));
		return;
	}

	UMoverComponent* Mover = Interactor->GetComponentByClass<UMoverComponent>();
	if (!IsValid(Mover))
	{
		UE_LOG(LogTemp, Warning, TEXT("APDTeleporter::OnInteract - Interactor has Invalid Mover!"));
		return;
	}

	USceneComponent* UpdatedComp = Mover->GetUpdatedComponent();
	if (!IsValid(UpdatedComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("APDTeleporter::OnInteract - Mover has Invalid Updated Component!"));
		return;
	}

	UpdatedComp->SetWorldLocationAndRotation(
		TeleportDest->Destination->GetComponentLocation(),
		TeleportDest->GetActorQuat(),
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
}

void APDTeleporter::OnEndInteract_Implementation(AActor* Interactor)
{
}
