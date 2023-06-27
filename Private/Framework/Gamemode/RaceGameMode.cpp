// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/RaceGameMode.h"

#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingPlayerState.h"
#include "UI/InGameHUD.h"

ARaceGameMode::ARaceGameMode()
	: APolyRacingGameModeBase()
{
}

void ARaceGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ARaceGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	APolyRacingPlayerController* JoiningPlayer = Cast<APolyRacingPlayerController>(NewPlayer);
	if (!JoiningPlayer)
		return;
	
	JoiningPlayer->Client_RequestVehicleSpawn();
}

void ARaceGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer); // prob remove

	APolyRacingPlayerState* PlayerState = NewPlayer->GetPlayerState<APolyRacingPlayerState>();
	if (!PlayerState)
		return;

	
}

void ARaceGameMode::AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints)
{
	CheckpointActors = Checkpoints;
	
	CheckpointActors.Sort([](const ACheckpointActor& CpA, const ACheckpointActor& CpB) {
		return  CpA.CheckpointNumber < CpB.CheckpointNumber;
	});	
}


