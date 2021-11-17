// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPTestGameMode.h"
#include "CPPTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACPPTestGameMode::ACPPTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACPPTestGameMode::RespawnNewPlayer()
{
	FTimerHandle    handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]()
		{
		GLog->Log("SpawnNew");
		FVector Location = playerSpawnPosition.GetLocation();
		FRotator Rotation = playerSpawnPosition.GetRotation().Rotator();
		ACPPTestCharacter* newPlayer = GetWorld()->SpawnActor<ACPPTestCharacter>(Location,Rotation);
		GetWorld()->GetFirstPlayerController()->Possess(newPlayer);
		}, timeBeforeSpawnNewPlayer, 0);
}

void ACPPTestGameMode::SetSpawnPosition(FTransform playerStartingTransform)
{
	playerSpawnPosition = playerStartingTransform;
}

