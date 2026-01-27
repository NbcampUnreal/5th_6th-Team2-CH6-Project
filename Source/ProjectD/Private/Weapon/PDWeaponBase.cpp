#include "Weapon/PDWeaponBase.h"

APDWeaponBase::APDWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	Muzzle->SetupAttachment(RootComp);
}

FVector APDWeaponBase::GetMuzzlePoint() const
{
	return Muzzle ? Muzzle->GetComponentLocation() : FVector::ZeroVector;
}
