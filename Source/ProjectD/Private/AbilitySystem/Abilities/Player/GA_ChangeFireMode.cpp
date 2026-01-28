#include "AbilitySystem/Abilities/Player/GA_ChangeFireMode.h"
#include "Components/Combat/WeaponManageComponent.h"
#include "Pawn/PDPawnBase.h"
#include "Weapon/PDWeaponBase.h"

UGA_ChangeFireMode::UGA_ChangeFireMode()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

void UGA_ChangeFireMode::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	
	APDPawnBase* OwnerPawn = ActorInfo ? Cast<APDPawnBase>(ActorInfo->AvatarActor.Get()) : nullptr;
	if (!OwnerPawn)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UWeaponManageComponent* WMC = OwnerPawn->FindComponentByClass<UWeaponManageComponent>();
	APDWeaponBase* Weapon = WMC ? WMC->GetEquippedWeapon() : nullptr;
	if (!Weapon || !Weapon->WeaponData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (HasAuthority(&ActivationInfo))
	{
		Weapon->ChangeFireMode();
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
