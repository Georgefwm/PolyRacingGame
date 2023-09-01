// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GameMode/FreeRoamGameMode.h"
#include "CheckpointActor.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingGameState.h"
#include "Framework/PolyRacingPlayerState.h"


// Sets default values
AFreeRoamGameMode::AFreeRoamGameMode()
	: APolyRacingGameModeBase()
{
	InitialSubState = MatchSubState::PreMainEvent;
}

void AFreeRoamGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AFreeRoamGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	if (!HasAuthority())
		return;
	
	for (APolyRacingPlayerController* PlayerController : GetGameState<APolyRacingGameState>()->ConnectedPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting defered players now"))
		HandleStartingNewPlayer_Implementation(PlayerController);
	}
}

void AFreeRoamGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AFreeRoamGameMode::HandleMainEventIsWaitingToStart()
{
	Super::HandleMainEventIsWaitingToStart();
}

void AFreeRoamGameMode::HandleMainEventHasStarted()
{
	OnCountDownSequenceEnd();
}

void AFreeRoamGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (GetMatchState() == MatchState::EnteringMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player start defered"))
		return;
	}
	
	APolyRacingPlayerController* JoiningPlayer = Cast<APolyRacingPlayerController>(NewPlayer);
	if (!JoiningPlayer)
		return;
	
	JoiningPlayer->UnPossess();
	JoiningPlayer->Client_RequestVehicleSpawn();  // Just spawn the players vehicle
}

void AFreeRoamGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AFreeRoamGameMode::AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints)
{
	CheckpointActors = Checkpoints;
	
	CheckpointActors.Sort([](const ACheckpointActor& CpA, const ACheckpointActor& CpB) {
		return  CpA.CheckpointNumber < CpB.CheckpointNumber;
	});	
}
