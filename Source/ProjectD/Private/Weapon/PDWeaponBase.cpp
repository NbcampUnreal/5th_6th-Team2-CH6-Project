#include "Weapon/PDWeaponBase.h"
#include "DataAssets/Weapon/DataAsset_Weapon.h"
#include "Net/UnrealNetwork.h"

APDWeaponBase::APDWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	Muzzle->SetupAttachment(RootComp);
}

void APDWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

FVector APDWeaponBase::GetMuzzlePoint() const
{
	return Muzzle ? Muzzle->GetComponentLocation() : FVector::ZeroVector;
}

bool APDWeaponBase::ServerCanFire(float Interval)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}
	
	// check remaining bullet and other conditions here if needed
	
	return true;
}

void APDWeaponBase::InitFireMode()
{
	if (WeaponData)
	{
		CurrentFireMode = WeaponData->DefaultFireMode;
	}
}
