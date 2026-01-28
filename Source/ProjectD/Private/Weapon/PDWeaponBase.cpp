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
	
	DOREPLIFETIME(APDWeaponBase, CurrentFireMode);
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

void APDWeaponBase::ChangeFireMode()
{
	const TArray<EPDWeaponFireMode>& Modes = WeaponData->SupportedFireModes;
	if (Modes.Num() <= 1)
	{
		return;
	}
	
	CurrentFireMode = GetNextFireMode();
}

EPDWeaponFireMode APDWeaponBase::GetNextFireMode() const
{
	if (!WeaponData)
	{
		return CurrentFireMode;
	}

	const TArray<EPDWeaponFireMode>& Modes = WeaponData->SupportedFireModes;
	if (Modes.Num() == 0)
	{
		return CurrentFireMode;
	}

	int32 Index = Modes.IndexOfByKey(CurrentFireMode);
	if (Index == INDEX_NONE)
	{
		if (Modes.Contains(WeaponData->DefaultFireMode))
		{
			return WeaponData->DefaultFireMode;
		}
		
		return Modes[0];
	}

	Index = (Index + 1) % Modes.Num();
	
	return Modes[Index];
}
