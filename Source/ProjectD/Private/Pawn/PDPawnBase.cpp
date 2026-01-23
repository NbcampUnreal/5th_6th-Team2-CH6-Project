#include "Pawn/PDPawnBase.h"

APDPawnBase::APDPawnBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

void APDPawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void APDPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

