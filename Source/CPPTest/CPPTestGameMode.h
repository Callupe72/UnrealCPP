// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPPTestCharacter.h"
#include "CPPTestGameMode.generated.h"

UCLASS(minimalapi)
class ACPPTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACPPTestGameMode();

	UPROPERTY(EditAnywhere, Category = Default)
	int timeBeforeSpawnNewPlayer = 5;
	
	UPROPERTY(EditAnywhere, Category = Default)
	ACharacter* playerToSpawn;

	UPROPERTY()
		FTransform playerSpawnPosition;

	UFUNCTION()
		void RespawnNewPlayer();
	UFUNCTION()
		void SetSpawnPosition(FTransform playerStartingTransform);
};



