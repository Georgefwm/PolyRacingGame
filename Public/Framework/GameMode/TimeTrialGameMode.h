// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CheckpointGameMode.h"
#include "GameFramework/GameMode.h"
#include "TimeTrialGameMode.generated.h"

/**
 * For now this GameMode is assumed to be single-player only
 */
UCLASS()
class POLYRACINGGAME_API ATimeTrialGameMode : public AGameMode, public ICheckpointGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimeTrialGameMode();

	UPROPERTY()
	TArray<class APolyRacingPlayerController*> ConnectedPlayers;

	UPROPERTY()
	TArray<class ACheckpointActor*> CheckpointActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleMatchIsWaitingToStart() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void RestartPlayer(AController* NewPlayer) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual void Logout(AController* Exiting) override;

	virtual void AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints) override;
};
