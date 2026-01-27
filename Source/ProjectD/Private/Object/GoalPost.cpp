#include "Object/GoalPost.h"
#include "Object/BallCore.h"
#include "Pawn/PDPawnBase.h" 
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

AGoalPost::AGoalPost()
{
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void AGoalPost::OnInteract_Implementation(AActor* Interactor)
{
	APDPawnBase* PDPawn = Cast<APDPawnBase>(Interactor);
	if (!PDPawn) return;

	if (PDPawn->GetCarriedBall())
	{
		Server_PlaceBall(PDPawn, PDPawn->GetCarriedBall());
	}
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

	Ball->Server_SetCarrier(Stealer);

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
	if (PlacedBall)
	{
		PlacedBall->Destroy();
		PlacedBall = nullptr;
	}
}