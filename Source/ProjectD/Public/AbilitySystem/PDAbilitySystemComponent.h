#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PDAbilitySystemComponent.generated.h"

UCLASS()
class PROJECTD_API UPDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);
};
