#include "Pawn/PDPawnBase.h"
#include "PlayerState/PDPlayerState.h"
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
#include "Object/BallCore.h"
#include "Object/GoalPost.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gimmick/PDInteractableObject.h"
#include "DrawDebugHelpers.h"

APDPawnBase::APDPawnBase()
{
	PrimaryActorTick.bCanEverTick = false;
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

void APDPawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APDPawnBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(Cast<UPDAbilitySystemComponent>(GetAbilitySystemComponent()));
		}
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


// =============

void APDPawnBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 헤더에 정의된 CarriedBall 변수 동기화
	DOREPLIFETIME(APDPawnBase, CarriedBall);
}

void APDPawnBase::TryInteract()
{
	// 1. 공을 들고 있다면 내려놓기 (Drop)
	if (CarriedBall)
	{
		Server_DropBall();
		return;
	}

	// 2. 공이 없다면 전방의 인터랙션 대상 찾기
	AActor* Target = FindInteractTarget();

	// 3. 대상이 인터페이스를 구현했다면 서버로 요청
	if (Target && Target->Implements<UPDInteractableObject>())
	{
		Server_TryInteract(Target);
	}
}

// [Server] 인터랙션 시도
void APDPawnBase::Server_TryInteract_Implementation(AActor* Target)
{
	if (Target && Target->Implements<UPDInteractableObject>())
	{
		// 인터페이스 호출 -> 대상(Ball/Goal)이 로직 수행 후 다시 Pawn의 함수 호출
		IPDInteractableObject::Execute_OnInteract(Target, this);
	}
}

// [Server] 공 줍기 (BallCore에서 호출됨)
void APDPawnBase::Server_PickUpBall_Implementation(ABallCore* Ball)
{
	if (!Ball || Ball->CarrierPawn) return;

	CarriedBall = Ball;

	// Ball에게 "내가 너를 들었다"고 알림 (물리 비활성화 등 처리)
	Ball->Server_SetCarrier(this);

	ApplyHoldingBallEffect();

	// 필요 시 추가 로직 (Server_OnBallPicked 호출 등)
	Server_OnBallPicked(Ball);
}

// [Server] 공 내려놓기
void APDPawnBase::Server_DropBall_Implementation()
{
	if (!CarriedBall) return;

	RemoveHoldingBallEffect();

	// [수정] 위쪽(UpVector)으로 50~100 정도 띄워서 바닥에 파묻히지 않게 함
	const FVector Forward = GetActorForwardVector();
	const FVector DropLoc = GetActorLocation() + (Forward * 100.f) + FVector(0.f, 0.f, 80.f);

	// 던지는 힘 (앞으로 + 약간 위로)
	const FVector Impulse = (Forward * 300.f) + (FVector::UpVector * 200.f);

	CarriedBall->Server_DropPhysics(DropLoc, Impulse);

	CarriedBall = nullptr;
	Server_OnBallRemoved();
}

// [Server] (헤더에 선언되어 있으므로 반드시 구현해야 함 - 링커 에러 방지)
void APDPawnBase::Server_OnBallPicked_Implementation(ABallCore* Ball)
{
	// 공을 주웠을 때 추가적으로 처리할 로직이 있다면 여기에 작성
	// 예: 점수 UI 갱신, 사운드 재생 요청 등
}

// [Server] (헤더에 선언되어 있으므로 반드시 구현해야 함 - 링커 에러 방지)
void APDPawnBase::Server_OnBallRemoved_Implementation()
{
	// 공을 잃었을 때(뺏기거나 떨궜을 때) 처리할 로직
}

AActor* APDPawnBase::FindInteractTarget() const
{
	UCameraComponent* Cam = FindComponentByClass<UCameraComponent>();
	if (!Cam) return nullptr;

	const FVector Start = Cam->GetComponentLocation();
	const FVector End = Start + Cam->GetForwardVector() * 450.f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// 반환값을 bHit 변수에 저장 (충돌 여부 확인용)
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, Start, End, ECC_Visibility, Params
	);

#if ENABLE_DRAW_DEBUG
	// 충돌했으면 초록색, 아니면 빨간색
	FColor DebugColor = bHit ? FColor::Green : FColor::Red;

	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		DebugColor,
		false, // bPersistentLines: true면 영원히 남음
		2.0f,  // LifeTime: 2초 동안 보임
		0,
		2.0f   // Thickness: 선 두께
	);

	// 만약 충돌 지점을 점으로 표시하고 싶다면 아래 주석 해제
	/*
	if (bHit)
	{
		DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 10.f, FColor::Green, false, 2.0f);
	}
	*/
#endif

	return Hit.GetActor();
}

void APDPawnBase::ApplyHoldingBallEffect()
{
	UAbilitySystemComponent* ASC = APDPawnBase::GetAbilitySystemComponent();

	if (ASC && GE_HoldingBall)
	{
		ASC->ApplyGameplayEffectToSelf(GE_HoldingBall.GetDefaultObject(), 1.f, ASC->MakeEffectContext());
	}
}

void APDPawnBase::RemoveHoldingBallEffect()
{
	UAbilitySystemComponent* ASC = APDPawnBase::GetAbilitySystemComponent();

	if (ASC)
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.HoldingBall")));
		ASC->RemoveActiveEffectsWithGrantedTags(TagContainer);
	}
}