#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structs/PDPlayerAbilitySet.h"
#include "DataAsset_Weapon.generated.h"

UENUM(BlueprintType)
enum class EPTWeaponType : uint8
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
	EPTWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FPDPlayerAbilitySet> GrantedAbilitySets;
};
