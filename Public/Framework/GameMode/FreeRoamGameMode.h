// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CheckpointGameMode.h"
#include "PolyRacingGameModeBase.h"
#include "FreeRoamGameMode.generated.h"

struct FPresetVehicleConfiguration;


UCLASS()
class POLYRACINGGAME_API AFreeRoamGameMode : public APolyRacingGameModeBase, public ICheckpointGameMode
{
	GENERATED_BODY()

public:
	
	AFreeRoamGameMode();
	
	virtual void BeginPlay() override;

	virtual void HandleMatchIsWaitingToStart() override;

	virtual void HandleMatchHasStarted() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void RestartPlayer(AController* NewPlayer) override;

	virtual void AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints) override;
};
