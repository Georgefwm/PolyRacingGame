// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GameMode/PolyRacingGameModeBase.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "CheckpointActor.h"
#include "PolyRacingWheeledVehiclePawn.h"
#include "Blueprint/UserWidget.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingGameState.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Subsystem/GameModeSubsystem.h"
#include "UI/InGameHUD.h"


// Sets default values
APolyRacingGameModeBase::APolyRacingGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseSeamlessTravel = false;
	
	PlayerControllerClass	= APolyRacingPlayerController::StaticClass();
	HUDClass				= AInGameHUD::StaticClass();

	PlayerStateClass		= APolyRacingPlayerState::StaticClass();
	GameStateClass			= APolyRacingGameState::StaticClass();
}

// Called when the game starts or when spawned
void APolyRacingGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APolyRacingGameModeBase::StartMatch()
{
	Super::StartMatch();

	BeginCountDownSequence();
}

void APolyRacingGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APolyRacingPlayerController* JoiningPlayer = Cast<APolyRacingPlayerController>(NewPlayer);
	if (!JoiningPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("APolyRacingGameMode: Invalid player tried to join the lobby"))
		return;
	}
	
	ConnectedPlayers.Add(JoiningPlayer);
	
	JoiningPlayer->GetPlayerState<APolyRacingPlayerState>()->bIsReady = false;
	
	UGameModeSubsystem* GameModeSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameModeSubsystem>();
	if (!GameModeSubsystem)
		return;
	
	JoiningPlayer->Client_SetGameMode(GameModeSubsystem->CurrentGameMode.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Setting player gamemode: %s"), *GameModeSubsystem->CurrentGameMode.ToString())

	// TODO: Make not 'hacky'
	if (HasAuthority())
		GetGameState<APolyRacingGameState>()->LapCount = GameModeSubsystem->GetCurrentGameModeLapCount();
}

void APolyRacingGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void APolyRacingGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	APolyRacingPlayerController* LobbyPlayerController = Cast<APolyRacingPlayerController>(Exiting);
	if (!LobbyPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("APolyRacingGameMode: Player logout error"))
		return;
	}
	
	ConnectedPlayers.Remove(LobbyPlayerController);
}

void APolyRacingGameModeBase::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void APolyRacingGameModeBase::RestartPlayerAtCheckpoint(APolyRacingPlayerController* PlayerController)
{
	int LastCheckpoint = PlayerController->GetPlayerState<APolyRacingPlayerState>()->LastCheckpoint;

	FTransform TeleportTransform;
	
	if (CheckpointActors.IsValidIndex(LastCheckpoint))
	{
		TeleportTransform = CheckpointActors[LastCheckpoint]->GetTransform();
	}
	else
	{
		TeleportTransform = CheckpointActors[0]->GetTransform();
	}

	FVector TeleportLocationOffset = FVector(0.0f, 0.0f, 20.0f);
	
	PlayerController->VehiclePawn->TeleportTo(TeleportTransform.GetLocation() + TeleportLocationOffset,
		TeleportTransform.GetRotation().Rotator());
	
	PlayerController->VehiclePawn->GetChaosVehicleMovementComponent()->ResetVehicleState();
}

bool APolyRacingGameModeBase::ReadyToStartMatch_Implementation()
{
	for (APolyRacingPlayerController* Player : ConnectedPlayers)
	{
		if (!Player->GetPlayerState<APolyRacingPlayerState>()->bIsReady)
			return false;
	}

	return true;
}

bool APolyRacingGameModeBase::ReadyToEndMatch_Implementation()
{
	for (APolyRacingPlayerController* Player : ConnectedPlayers)
	{
		// Have all players completed the final lap?
		if (Player->GetPlayerState<APolyRacingPlayerState>()->EventEndTime < 0.2f)
			return false;
	}

	return true;
}

void APolyRacingGameModeBase::CheckIfShouldStart()
{
	if (MatchState == MatchState::InProgress)
		return;
	
	if (ReadyToStartMatch())
		StartMatch();
}

void APolyRacingGameModeBase::CheckIfShouldEnd()
{
	if (MatchState != MatchState::InProgress)
		return;
	
	if (ReadyToEndMatch())
		EndMatch();
}

void APolyRacingGameModeBase::BeginCountDownSequence()
{
	for (APolyRacingPlayerController* Player : ConnectedPlayers)
	{
		Player->Client_PlayCountDown();

		if (APolyRacingWheeledVehiclePawn* VehiclePawn = Player->GetPawn<APolyRacingWheeledVehiclePawn>())
			VehiclePawn->DisableInput(Player);
	}

	// @ASSUMPTION : Count down animation sequence is 3 seconds long
	float constexpr CountDownAnimationDuration = 3.0f;
	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &APolyRacingGameModeBase::OnCountDownSequenceEnd,
		CountDownAnimationDuration, false);
}

void APolyRacingGameModeBase::OnCountDownSequenceEnd()
{
	for (APolyRacingPlayerController* Player : ConnectedPlayers)
		Player->Client_OnCountDownSequenceEnd();
}

void APolyRacingGameModeBase::HandlePlayerHasFinishedEvent(APolyRacingPlayerController* PlayerController)
{
	APolyRacingWheeledVehiclePawn* PlayerPawn = PlayerController->GetPawn<APolyRacingWheeledVehiclePawn>();
	if (!PlayerPawn)
		return;

	PlayerController->GetPawn()->DisableInput(PlayerController);
	
	if (EndEventWidget)
		PlayerController->AddWidgetToScreen(EndEventWidget);

	CheckIfShouldEnd();

	// UMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	//  
	// if (ULevelSequence* Sequence = MapSubsystem->GetCurrentLevelOutroSequence())
	// 	PlayerController->Client_PlayLevelOutroSequence(Sequence);
}

void APolyRacingGameModeBase::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	if (ConnectedPlayers.Num() <= 1)
	{
		for (APolyRacingPlayerController* PlayerController : ConnectedPlayers)
		{
			FTimerHandle LeaveGameTimerHandle = FTimerHandle();
			GetWorld()->GetTimerManager().SetTimer(LeaveGameTimerHandle,
				PlayerController,
				&APolyRacingPlayerController::StartLeavingMatchSinglePlayer,
				5.f,
				false);
		}
	}
}

