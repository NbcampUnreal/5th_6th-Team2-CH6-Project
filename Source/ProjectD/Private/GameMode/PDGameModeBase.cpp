#include "GameMode/PDGameModeBase.h"

APDGameModeBase::APDGameModeBase()
{
}

void APDGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	const UWorld* World = GetWorld();
	UE_LOG(LogTemp, Warning, TEXT("[Server GM] Map=%s GM=%s DefaultPawnClass=%s"),
		World ? *World->GetMapName() : TEXT("None"),
		*GetNameSafe(GetClass()),
		*GetNameSafe(DefaultPawnClass));
}
