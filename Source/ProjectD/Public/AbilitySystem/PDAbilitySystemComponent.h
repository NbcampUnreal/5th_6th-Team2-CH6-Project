#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PDAbilitySystemComponent.generated.h"

class APDWeaponBase;
class UAbilitySystemComponent;

UCLASS()
class PROJECTD_API UPDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);
	
	void ProcessAbilityInput(float DeltaTime);

private:
	void TryActivateByInputTag(const FGameplayTag& InputTag);
	
	APDWeaponBase* GetEquippedWeaponFromASC(const UAbilitySystemComponent* ASC) const;
	bool IsEquippedWeaponFullAuto(const UAbilitySystemComponent* ASC) const;
	
private:
	TSet<FGameplayTag> HoldInputTags;
	TMap<FGameplayTag, float> NextTryTimeByTag;
};
