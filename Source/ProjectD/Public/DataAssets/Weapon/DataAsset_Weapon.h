#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structs/PDPlayerAbilitySet.h"
#include "DataAsset_Weapon.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EPDWeaponType : uint8
{
	None,
	Pistol,
	Rifle,
	Shotgun,
	Throwable
};

UCLASS()
class PROJECTD_API UDataAsset_Weapon : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPDWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FPDPlayerAbilitySet> GrantedAbilitySets;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> WeaponDamageGE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponDamage = 30.f;
	
	float MaxRange = 10000.f;
};
