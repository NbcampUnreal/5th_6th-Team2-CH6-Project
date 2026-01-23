#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "PDPawnBase.generated.h"

class UWeaponManageComponent;
class USkillManageComponent;
class UDataAsset_InputConfig;
class UDataAsset_StartUpBase;
struct FInputActionValue;

UCLASS()
class PROJECTD_API APDPawnBase : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APDPawnBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UWeaponManageComponent* GetWeaponManageComponent() const { return WeaponManageComponent; }
	FORCEINLINE USkillManageComponent* GetSkillManageComponent() const { return SkillManageComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitAbilityActorInfo();

private:
	void Input_AbilityInputPressed(FGameplayTag InputTag);
	void Input_AbilityInputReleased(FGameplayTag InputTag);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UWeaponManageComponent> WeaponManageComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkillManageComponent> SkillManageComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpBase> CharacterStartUpData;
};
