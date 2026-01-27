#include "AbilitySystem/PDAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PDGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Pawn/PDPawnBase.h"
#include "Weapon/PDWeaponBase.h"
#include "DataAssets/Weapon/DataAsset_Weapon.h"
#include "Components/Combat/WeaponManageComponent.h"
#include "PDGameplayTags.h"

void UPDAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}
	
	TryActivateByInputTag(InInputTag);
	
	HoldInputTags.Add(InInputTag);
	NextTryTimeByTag.FindOrAdd(InInputTag, 0.f);
}

void UPDAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}
	
	HoldInputTags.Remove(InInputTag);
	NextTryTimeByTag.Remove(InInputTag);
}

void UPDAbilitySystemComponent::TryActivateByInputTag(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	
	TArray<FGameplayAbilitySpecHandle> HandlesToActivate;
	{
		FScopedAbilityListLock AbilityListLock(*this);
		for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
		{
			if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
			{
				if (!Spec.IsActive())
				{
					HandlesToActivate.Add(Spec.Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& Handle : HandlesToActivate)
	{
		TryActivateAbility(Handle);
	}
}

float UPDAbilitySystemComponent::GetFireIntervalFromEquippedWeapon() const
{
	const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return 0.1f;
	}

	const APDPawnBase* Pawn = Cast<APDPawnBase>(ActorInfo->AvatarActor.Get());
	if (!Pawn)
	{
		return 0.1f;
	}

	const UWeaponManageComponent* WMC = Pawn->FindComponentByClass<UWeaponManageComponent>();
	const APDWeaponBase* Weapon = WMC ? WMC->GetEquippedWeapon() : nullptr;
	if (!Weapon || !Weapon->WeaponData)
	{
		return 0.1f;
	}

	return Weapon->WeaponData->FireInterval;
}

void UPDAbilitySystemComponent::ProcessAbilityInput(float DeltaTime)
{
	UWorld* World = GetWorld();
	if (!World) return;

	if (!HoldInputTags.Contains(PDGameplayTags::InputTag_Weapon_Fire))
	{
		return;
	}

	const float Now = World->GetTimeSeconds();
	float& NextTry = NextTryTimeByTag.FindOrAdd(PDGameplayTags::InputTag_Weapon_Fire, 0.f);

	constexpr float PollInterval = 0.02f;
	if (Now < NextTry)
	{
		return;
	}
	
	NextTry = Now + PollInterval;

	TryActivateByInputTag(PDGameplayTags::InputTag_Weapon_Fire);
}