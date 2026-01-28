#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PDPlayerGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GA_Fire.generated.h"

class APDPawnBase;
class APDWeaponBase;
struct FGameplayAbilityTargetDataHandle;
struct FHitResult;

UCLASS()
class PROJECTD_API UGA_Fire : public UPDPlayerGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Fire();
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	
protected:
	void ApplyWeaponDamageGE(const FHitResult& Hit, const APDWeaponBase* Weapon);
	
	void OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);
	void MuzzleTraceAndApplyGE(APDPawnBase* OwnerPawn, APDWeaponBase* Weapon, const FVector& AimPoint);
	static FGameplayAbilityTargetDataHandle MakeAimPointTargetData(const FVector& CameraStart, const FVector& AimPoint);
};
