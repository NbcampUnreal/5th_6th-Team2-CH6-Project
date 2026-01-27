#include "AbilitySystem/PDAbilitySystemComponent.h"

void UPDAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> HandlesToActivate;
	{
		FScopedAbilityListLock AbilityListLock(*this);

		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
			{
				HandlesToActivate.Add(AbilitySpec.Handle);
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& Handle : HandlesToActivate)
	{
		TryActivateAbility(Handle);
	}
}

void UPDAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}
