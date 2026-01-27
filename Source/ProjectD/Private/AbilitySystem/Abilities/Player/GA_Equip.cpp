#include "AbilitySystem/Abilities/Player/GA_Equip.h"
#include "Components/Combat/WeaponManageComponent.h"

void UGA_Equip::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	
	UWeaponManageComponent* WMC = GetWeaponManageComponentFromActorInfo();
	if (!WMC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (!WMC->GetWeaponInSlot(EquipSlotIndex))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (HasAuthority(&ActivationInfo))
	{
		WMC->EquipSlot(EquipSlotIndex);
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
