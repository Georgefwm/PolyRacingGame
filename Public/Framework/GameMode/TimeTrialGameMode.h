// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CheckpointGameMode.h"
#include "PolyRacingGameModeBase.h"
#include "TimeTrialGameMode.generated.h"


/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API ATimeTrialGameMode : public APolyRacingGameModeBase, public ICheckpointGameMode
{
	GENERATED_BODY()

public:
	
	ATimeTrialGameMode();
	
	virtual void BeginPlay() override;

	virtual void HandleMatchIsWaitingToStart() override;

	virtual void HandleMatchHasStarted() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void RestartPlayer(AController* NewPlayer) override;

	virtual void AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints) override;

	virtual void BeginCountDownSequence() override;

	virtual void HandlePlayerHasFinishedEvent(APolyRacingPlayerController* PlayerController) override;
};
