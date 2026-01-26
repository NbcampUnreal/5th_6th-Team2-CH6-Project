#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structs/PDPlayerAbilitySet.h"
#include "DataAsset_Weapon.generated.h"

UENUM(BlueprintType)
enum class EPDWeaponType : uint8
{
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
	
	float MaxRange = 10000.f;
};
