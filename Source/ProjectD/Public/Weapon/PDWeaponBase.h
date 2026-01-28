#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/PDWeaponTypes.h"
#include "PDWeaponBase.generated.h"

class UDataAsset_Weapon;

UCLASS()
class PROJECTD_API APDWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APDWeaponBase();

	FORCEINLINE EPDWeaponFireMode GetCurrentFireMode() const { return CurrentFireMode; }
	FORCEINLINE bool IsFullAuto() const { return CurrentFireMode == EPDWeaponFireMode::FullAuto; }
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FVector GetMuzzlePoint() const;
	
	bool ServerCanFire(float Interval);
	
	void InitFireMode();
	void ChangeFireMode();
	
private:
	EPDWeaponFireMode GetNextFireMode() const;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataAsset_Weapon> WeaponData;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Muzzle;
	
private:
	UPROPERTY(Replicated)
	EPDWeaponFireMode CurrentFireMode = EPDWeaponFireMode::SemiAuto;
};
