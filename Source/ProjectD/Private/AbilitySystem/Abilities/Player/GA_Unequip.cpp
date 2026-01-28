#include "AbilitySystem/Abilities/Player/GA_Unequip.h"
#include "Components/Combat/WeaponManageComponent.h"

void UGA_Unequip::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (UWeaponManageComponent* WMC = GetWeaponManageComponentFromActorInfo())
	{
		WMC->UnequipCurrentWeapon();
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
