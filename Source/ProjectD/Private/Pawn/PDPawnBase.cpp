#include "Pawn/PDPawnBase.h"
#include "PlayerState/PDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/PDAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Input/PDEnhancedInputComponent.h"
#include "Components/Combat/WeaponManageComponent.h"
#include "Components/Combat/SkillManageComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "GameplayTagContainer.h"
#include "PDGameplayTags.h"
#include "DataAssets/StartUp/DataAsset_StartUpBase.h"
#include "AttributeSet/PDAttributeSetBase.h"

APDPawnBase::APDPawnBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	WeaponManageComponent = CreateDefaultSubobject<UWeaponManageComponent>(TEXT("WeaponManageComponent"));
	SkillManageComponent = CreateDefaultSubobject<USkillManageComponent>(TEXT("SkillManageComponent"));

	OverrideInputComponentClass = UPDEnhancedInputComponent::StaticClass();
}

UAbilitySystemComponent* APDPawnBase::GetAbilitySystemComponent() const
{
	if (APDPlayerState* PDPlayerState = GetPlayerState<APDPlayerState>())
	{
		return PDPlayerState->GetAbilitySystemComponent();
	}

	return nullptr;
}

USkeletalMeshComponent* APDPawnBase::GetSkeletalMeshComponent() const
{
	return FindComponentByClass<USkeletalMeshComponent>();
}

void APDPawnBase::ClientDrawFireDebug_Implementation(
	const FVector& Start,
	const FVector& End,
	bool bHit,
	const FVector& HitPoint
)
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, 0, 1.f);
	if (bHit)
	{
		DrawDebugPoint(GetWorld(), HitPoint, 8.f, FColor::Yellow, false, 1.f);
	}
}

void APDPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (UPDAbilitySystemComponent* ASC = Cast<UPDAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->ProcessAbilityInput(DeltaTime);
	}
}

void APDPawnBase::BeginPlay()
{
	Super::BeginPlay();
}

void APDPawnBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	
	if (HasAuthority())
	{
		if (!CharacterStartUpData.IsNull())
		{
			if (UDataAsset_StartUpBase* LoadedData = CharacterStartUpData.LoadSynchronous())
			{
				LoadedData->GiveToAbilitySystemComponent(Cast<UPDAbilitySystemComponent>(GetAbilitySystemComponent()));
			}
		}
		
		InitAttributeSet();
	}
}

void APDPawnBase::OnRep_PlayerState()
{
	InitAbilityActorInfo();
}

void APDPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(InputConfigDataAsset->AbilityMappingContext, 1);
		}
	}

	UPDEnhancedInputComponent* PDInputComponent = CastChecked<UPDEnhancedInputComponent>(PlayerInputComponent);
	PDInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void APDPawnBase::InitAbilityActorInfo()
{
	APDPlayerState* PDPlayerState = GetPlayerState<APDPlayerState>();
	if (!PDPlayerState)
	{
		return;
	}

	PDPlayerState->InitAbilityActorInfo(this);
}

void APDPawnBase::InitAttributeSet()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("APDPawnBase::InitAttributeSet - ASC is not valid"));
		return;
	}
	
	APDPlayerState* PDPlayerState = GetPlayerState<APDPlayerState>();
	if (!PDPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("APDPawnBase::InitAttributeSet - PlayerState is not valid"));
		return;
	}
	
	ASC->AddAttributeSetSubobject<UPDAttributeSetBase>(PDPlayerState->GetPDAttributeSetBase());
	
	BindAttributeChangeDelegates();
}

void APDPawnBase::BindAttributeChangeDelegates()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("APDPawnBase::InitAttributeSet - ASC is not valid"));
		return;
	}
	
	ASC->GetGameplayAttributeValueChangeDelegate(UPDAttributeSetBase::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void APDPawnBase::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("APDPawnBase::OnHealthChanged - New Health: %f"), Data.NewValue);
}

void APDPawnBase::OnMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("APDPawnBase::OnMoveSpeedChanged - New Move Speed: %f"), Data.NewValue);
}

void APDPawnBase::Input_AbilityInputPressed(FGameplayTag InputTag)
{
	if (UPDAbilitySystemComponent* ASC = Cast<UPDAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->OnAbilityInputPressed(InputTag);
	}
}

void APDPawnBase::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	if (UPDAbilitySystemComponent* ASC = Cast<UPDAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->OnAbilityInputReleased(InputTag);
	}
}