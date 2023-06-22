// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/TimeTrialGameMode.h"

#include "LevelSequence.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingPlayerState.h"
#include "String/Join.h"
#include "Subsystem/MapSubsystem.h"
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

void ATimeTrialGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	if (!HasAuthority())
		return;

	
	for (APolyRacingPlayerController* PlayerController : ConnectedPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting defered player now"))
		HandleStartingNewPlayer_Implementation(PlayerController);
	}
}

void ATimeTrialGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (GetMatchState() == MatchState::EnteringMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player start defered"))
		return;
	}
	
	APolyRacingPlayerController* JoiningPlayer = Cast<APolyRacingPlayerController>(NewPlayer);
	if (!JoiningPlayer)
		return;
	
	JoiningPlayer->UnPossess(); // Necessary for playing level sequence

	UMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	 
	if (ULevelSequence* Sequence = MapSubsystem->GetCurrentLevelSequence())
		JoiningPlayer->Client_PlayLevelIntroSequence(Sequence);  // Play intro sequence if one is set,
	else
		JoiningPlayer->Client_RequestVehicleSpawn();  // Just spawn the players vehicle if no sequence found
}

void ATimeTrialGameMode::RestartPlayer(AController* NewPlayer)
{
	//Super::RestartPlayer(NewPlayer); // prob remove

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
}

