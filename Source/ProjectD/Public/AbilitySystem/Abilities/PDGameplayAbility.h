#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PDGameplayAbility.generated.h"

class UPDAbilitySystemComponent;

UENUM(BlueprintType)
enum class EPTAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

UCLASS()
class PROJECTD_API UPDGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void OnGiveAbility(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec
	) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;
	
	UFUNCTION(BlueprintPure, Category = "ASC")
	UPDAbilitySystemComponent* GetPDAbilitySystemComponentFromActorInfo() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "MA|Ability")
	EPTAbilityActivationPolicy AbilityActivationPolicy = EPTAbilityActivationPolicy::OnTriggered;
};
