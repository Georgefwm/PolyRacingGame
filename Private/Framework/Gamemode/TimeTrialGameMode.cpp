// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/TimeTrialGameMode.h"

#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingPlayerState.h"
#include "UI/InGameHUD.h"

ATimeTrialGameMode::ATimeTrialGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseSeamlessTravel = false;
	
	PlayerControllerClass = APolyRacingPlayerController::StaticClass();
	HUDClass = AInGameHUD::StaticClass();

	PlayerStateClass = APolyRacingPlayerState::StaticClass();
}

void ATimeTrialGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATimeTrialGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	APolyRacingPlayerController* JoiningPlayer = Cast<APolyRacingPlayerController>(NewPlayer);
	if (!JoiningPlayer)
		return;
	
	JoiningPlayer->Client_RequestVehicleSpawn();
}

void ATimeTrialGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer); // prob remove

	APolyRacingPlayerState* PlayerState = NewPlayer->GetPlayerState<APolyRacingPlayerState>();
	if (!PlayerState)
		return;

	
}

void ATimeTrialGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APolyRacingPlayerController* JoiningPlayer = Cast<APolyRacingPlayerController>(NewPlayer);
	if (!JoiningPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAMEMODE: invalid player tried to join the lobby"))
		return;
	}
	
	ConnectedPlayers.Add(JoiningPlayer);
}

void ATimeTrialGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ATimeTrialGameMode::AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints)
{
	CheckpointActors = Checkpoints;
	
	CheckpointActors.Sort([](const ACheckpointActor& CpA, const ACheckpointActor& CpB) {
		return  CpA.CheckpointNumber < CpB.CheckpointNumber;
	});

	for (ACheckpointActor* Checkpoint : CheckpointActors)
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint %i"), Checkpoint->CheckpointNumber)
	
}

