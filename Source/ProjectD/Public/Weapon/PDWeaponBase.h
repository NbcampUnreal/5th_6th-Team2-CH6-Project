#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDWeaponBase.generated.h"

class UDataAsset_Weapon;

UCLASS()
class PROJECTD_API APDWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APDWeaponBase();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FVector GetMuzzlePoint() const;
	
	bool ServerCanFire(float Interval);
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataAsset_Weapon> WeaponData;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Muzzle;
};
