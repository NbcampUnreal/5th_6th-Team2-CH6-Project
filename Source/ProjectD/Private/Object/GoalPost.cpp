#include "Object/GoalPost.h"
#include "Object/BallCore.h"
#include "Pawn/PDPawnBase.h" // 폰 상태 확인용
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

AGoalPost::AGoalPost()
{
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

// [인터페이스 구현] 헤더에 선언된 _Implementation 함수 정의
void AGoalPost::OnInteract_Implementation(AActor* Interactor)
{
	APDPawnBase* PDPawn = Cast<APDPawnBase>(Interactor);
	if (!PDPawn) return;

	UE_LOG(LogTemp, Warning, TEXT("Post"));

	// 폰이 공을 들고 있다면 -> 골대에 넣기
	if (PDPawn->GetCarriedBall())
	{
		Server_PlaceBall(PDPawn, PDPawn->GetCarriedBall());
	}
	// 폰이 빈손이고, 골대에 공이 있다면 -> 공 뺏기
	else if (PlacedBall)
	{
		Server_StealBall(PDPawn);
	}
}

bool AGoalPost::CanPlaceBall(APawn* Pawn, ABallCore* Ball) const
{
	return Pawn && Ball && !PlacedBall;
}

void AGoalPost::Server_PlaceBall(APawn* Pawn, ABallCore* Ball)
{
	if (!HasAuthority()) return;
	if (!CanPlaceBall(Pawn, Ball)) return;

	PlacedBall = Ball;

	// 공의 주인 정보를 지움 (물리 비활성화 등은 BallCore 내부 로직에 의해 처리됨)
	Ball->Server_ClearCarrier();

	Ball->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	StartHoldTimer();
}

void AGoalPost::Server_StealBall(APawn* Stealer)
{
	if (!HasAuthority()) return;
	if (!PlacedBall || !Stealer) return;

	GetWorld()->GetTimerManager().ClearTimer(HoldTimer);

	ABallCore* Ball = PlacedBall;
	PlacedBall = nullptr;

	// 뺏는 사람에게 공을 쥐어줌 (BallCore::Server_SetCarrier -> Attach 처리됨)
	Ball->Server_SetCarrier(Stealer);

	// 폰 쪽에도 상태 업데이트가 필요하다면 호출 (선택 사항, 폰 로직에 따라 다름)
	if (APDPawnBase* PD = Cast<APDPawnBase>(Stealer))
	{
		PD->Server_OnBallPicked(Ball);
	}
}

void AGoalPost::StartHoldTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		HoldTimer, this, &AGoalPost::OnHoldComplete, 10.f, false
	);
}

void AGoalPost::OnHoldComplete()
{
	// 점수 지급 로직 등
	if (PlacedBall)
	{
		PlacedBall->Destroy();
		PlacedBall = nullptr;
	}
}