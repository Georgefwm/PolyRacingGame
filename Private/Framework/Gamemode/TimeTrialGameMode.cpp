// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GameMode/TimeTrialGameMode.h"

#include "CheckpointActor.h"
#include "LevelSequence.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Subsystem/MapSubsystem.h"


ATimeTrialGameMode::ATimeTrialGameMode()
	: APolyRacingGameModeBase()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> EndEventWidgetFinder(TEXT("/Game/UI/WidgetBlueprints/WBP_ResultTime"));
	EndEventWidget = EndEventWidgetFinder.Class;

	SubState = MatchSubState::PreMainEvent;
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
		UE_LOG(LogTemp, Warning, TEXT("Starting defered players now"))
		HandleStartingNewPlayer_Implementation(PlayerController);
	}
}

void ATimeTrialGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	// for (APolyRacingPlayerController* Player : ConnectedPlayers)
	// {
	// 	EnableInput(Player);
	// }
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
	 
	if (ULevelSequence* Sequence = MapSubsystem->GetCurrentLevelIntroSequence())
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

void ATimeTrialGameMode::AddCheckpoints(TArray<ACheckpointActor*>& Checkpoints)
{
	CheckpointActors = Checkpoints;
	
	CheckpointActors.Sort([](const ACheckpointActor& CpA, const ACheckpointActor& CpB) {
		return  CpA.CheckpointNumber < CpB.CheckpointNumber;
	});	
}

void ATimeTrialGameMode::BeginCountDownSequence()
{
	Super::BeginCountDownSequence();
}

void ATimeTrialGameMode::HandlePlayerHasFinishedEvent(APolyRacingPlayerController* PlayerController)
{
	Super::HandlePlayerHasFinishedEvent(PlayerController);
}

