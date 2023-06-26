// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CheckpointGameMode.h"
#include "PolyRacingGameModeBase.h"
#include "GameFramework/GameMode.h"
#include "TimeTrialGameMode.generated.h"

/**
 * @ASSUMPTION : Single player only game mode
 */
UCLASS()
class POLYRACINGGAME_API ATimeTrialGameMode : public APolyRacingGameModeBase, public ICheckpointGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimeTrialGameMode();

	UPROPERTY()
	TArray<class ACheckpointActor*> CheckpointActors;

	UPROPERTY()
	TArray<float> LapTimes;

	UPROPERTY()
	float TotalTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleMatchIsWaitingToStart() override;

	virtual void HandleMatchHasStarted() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void RestartPlayer(AController* NewPlayer) override;

	virtual void AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints) override;

	virtual void BeginCountDownSequence() override;
	
};
