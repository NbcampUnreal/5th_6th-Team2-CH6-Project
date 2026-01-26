#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "PDPlayerState.generated.h"

class UPDAbilitySystemComponent;

UCLASS()
class PROJECTD_API APDPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	APDPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void InitAbilityActorInfo(AActor* AvatarActor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UPDAbilitySystemComponent> AbilitySystemComponent;
};
