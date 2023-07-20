// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GameMode/RaceGameMode.h"

#include "CheckpointActor.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Subsystem/MapSubsystem.h"


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
	
	JoiningPlayer->UnPossess(); // Necessary for playing level sequence

	UMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	 
	if (ULevelSequence* Sequence = MapSubsystem->GetCurrentLevelIntroSequence())
		JoiningPlayer->Client_PlayLevelIntroSequence(Sequence);  // Play intro sequence if one is set,
	else
		JoiningPlayer->Client_RequestVehicleSpawn();  // Just spawn the players vehicle if no sequence found
}

void ARaceGameMode::StartMatch()
{
	Super::StartMatch();

	SubState = MatchSubState::Qualifier;

	HandleQualifierHasStarted();
}

void ARaceGameMode::HandleQualifierHasStarted()
{
	BeginCountDownSequence();
}

void ARaceGameMode::HandleQualifierHasEnded()
{
}

void ARaceGameMode::HandleMainEventHasStarted()
{
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


