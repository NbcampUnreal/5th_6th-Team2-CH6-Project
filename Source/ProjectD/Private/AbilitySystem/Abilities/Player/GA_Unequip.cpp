#include "AbilitySystem/Abilities/Player/GA_Unequip.h"
#include "Components/Combat/WeaponManageComponent.h"

UGA_Unequip::UGA_Unequip()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

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
