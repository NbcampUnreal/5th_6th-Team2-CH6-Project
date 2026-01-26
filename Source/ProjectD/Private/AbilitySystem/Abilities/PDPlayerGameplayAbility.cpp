#include "AbilitySystem/Abilities/PDPlayerGameplayAbility.h"
#include "Controller/PDPlayerController.h"
#include "Pawn/PDPawnBase.h"
#include "Components/Combat/WeaponManageComponent.h"

APDPawnBase* UPDPlayerGameplayAbility::GetPlayerPawnFromActorInfo()
{
	if (!CachedPlayerPawn.IsValid())
	{
		CachedPlayerPawn = Cast<APDPawnBase>(GetAvatarActorFromActorInfo());
	}
	
	return CachedPlayerPawn.IsValid() ? CachedPlayerPawn.Get() : nullptr;
}

APDPlayerController* UPDPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!CachedPlayerController.IsValid())
	{
		CachedPlayerController = Cast<APDPlayerController>(GetPlayerPawnFromActorInfo()->GetController());
	}
	
	return CachedPlayerController.IsValid() ? CachedPlayerController.Get() : nullptr;
}

UWeaponManageComponent* UPDPlayerGameplayAbility::GetWeaponManageComponentFromActorInfo()
{
	return GetPlayerPawnFromActorInfo()->GetWeaponManageComponent();
}
