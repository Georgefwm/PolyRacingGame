// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CheckpointGameMode.h"
#include "PolyRacingGameModeBase.h"
#include "RaceGameMode.generated.h"


/**
 * For now this GameMode is assumed to be single-player only
 */
UCLASS()
class POLYRACINGGAME_API ARaceGameMode : public APolyRacingGameModeBase, public ICheckpointGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARaceGameMode();

	UPROPERTY()
	TArray<class ACheckpointActor*> CheckpointActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void RestartPlayer(AController* NewPlayer) override;

	virtual void AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints) override;
};
