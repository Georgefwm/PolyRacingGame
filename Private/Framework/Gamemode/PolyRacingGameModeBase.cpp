// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GameMode/PolyRacingGameModeBase.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "CheckpointActor.h"
#include "PolyRacingWheeledVehiclePawn.h"
#include "StartPositionActor.h"
#include "Blueprint/UserWidget.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingGameState.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/GameModeSubsystem.h"
#include "UI/InGameHUD.h"


namespace MatchSubState
{
	const FName Qualifier     = FName(TEXT("Qualifier"));
	const FName PostQualifier = FName(TEXT("PostQualifier"));
	const FName PreMainEvent  = FName(TEXT("PreMainEvent"));
	const FName MainEvent     = FName(TEXT("MainEvent"));
}


// Sets default values
APolyRacingGameModeBase::APolyRacingGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseSeamlessTravel = false;
	
	PlayerControllerClass	= APolyRacingPlayerController::StaticClass();
	HUDClass				= AInGameHUD::StaticClass();

	PlayerStateClass		= APolyRacingPlayerState::StaticClass();
	GameStateClass			= APolyRacingGameState::StaticClass();

	InitialSubState			= MatchSubState::PreMainEvent;
}

void APolyRacingGameModeBase::SetMatchSubState(FName NewState)
{
	SubState = NewState;
	UE_LOG(LogTemp, Warning, TEXT("MatchSubState changed to: %s"), *SubState.ToString())
	
	if		(SubState == MatchSubState::Qualifier)     HandleQualifierHasStarted();
	else if (SubState == MatchSubState::PostQualifier) HandleQualifierHasEnded();
	else if (SubState == MatchSubState::PreMainEvent)  HandleMainEventIsWaitingToStart();
	else if (SubState == MatchSubState::MainEvent)     HandleMainEventHasStarted();
}

// Called when the game starts or when spawned
void APolyRacingGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void APolyRacingGameModeBase::StartMatch()
{
	Super::StartMatch();
	
	// Default Value of SubState is the starting state
	SetMatchSubState(InitialSubState);
}

void APolyRacingGameModeBase::HandleQualifierHasStarted()
{
	BeginCountDownSequence();
}

void APolyRacingGameModeBase::HandleQualifierHasEnded()
{
	for (APolyRacingPlayerController* PlayerController : ConnectedPlayers)
	{
		// TODO: Set to Spectator camera
		PlayerController->UnPossess();
		PlayerController->GetPlayerState<APolyRacingPlayerState>()->ResetForNextRace();
	}
	
	SetMatchSubState(MatchSubState::PreMainEvent);
}

void APolyRacingGameModeBase::HandleMainEventIsWaitingToStart()
{
	TArray<AActor*> StartPositions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStartPositionActor::StaticClass(), StartPositions);
	
	if (!StartPositions.IsEmpty())
		Cast<AStartPositionActor>(StartPositions[0])->ResetSpawnCount();
	
	for (APolyRacingPlayerController* PlayerController : ConnectedPlayers)
	{
		// if (PlayerController->VehiclePawn)
		// 	PlayerController->VehiclePawn->Destroy();
		
		PlayerController->Client_RequestVehicleSpawn();
	}
}

void APolyRacingGameModeBase::HandleMainEventHasStarted()
{
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
	if (APolyRacingPlayerController* PlayerController = Cast<APolyRacingPlayerController>(NewPlayer))
	{
		PlayerController->GetPlayerState<APolyRacingPlayerState>()->bIsReady = true;
	}
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
		UE_LOG(LogTemp, Warning, TEXT("Checkpoints disabled for current game mode"))
		// TeleportTransform = CheckpointActors[0]->GetTransform();
	}

	FVector TeleportLocationOffset = FVector(0.0f, 0.0f, 30.0f);

	PlayerController->VehiclePawn->GetChaosVehicleMovementComponent()->StopMovementImmediately();

	PlayerController->VehiclePawn->SetActorLocationAndRotation(
		TeleportTransform.GetLocation() + TeleportLocationOffset,
		TeleportTransform.GetRotation().Rotator(),
		false,
		nullptr,
		ETeleportType::TeleportPhysics);
	
	PlayerController->VehiclePawn->GetChaosVehicleMovementComponent()->StopMovementImmediately();
}

bool APolyRacingGameModeBase::ReadyToStartMatch_Implementation()
{
	for (APolyRacingPlayerController* Player : ConnectedPlayers)
	{
		if (!Player->GetPlayerState<APolyRacingPlayerState>()->bIsReady)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not ready to start match"))
			return false;
		}
			
	}

	return true;
}

bool APolyRacingGameModeBase::ReadyToEndMatch_Implementation()
{
	for (const APolyRacingPlayerController* Player : ConnectedPlayers)
	{
		// Have all players completed the event?
		if (Player->GetPlayerState<APolyRacingPlayerState>()->EventEndTime < 0.2f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not ready to end match"))
			return false;
		}
			
	}

	return true;
}

void APolyRacingGameModeBase::BeginCountDownSequence()
{
	for (APolyRacingPlayerController* Player : ConnectedPlayers)
		Player->Client_PlayCountDown();
	
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
}

void APolyRacingGameModeBase::EndMatch()
{
	Super::EndMatch();

	HandleMatchHasEnded();
}

void APolyRacingGameModeBase::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	UE_LOG(LogTemp, Warning, TEXT("HANDLEMATCHHASENDED"))
	
	for (APolyRacingPlayerController* PlayerController : ConnectedPlayers)
	{		
		FTimerHandle LeaveGameTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(LeaveGameTimerHandle,
			PlayerController,
			&APolyRacingPlayerController::StartLeavingMatchSinglePlayer,
			5.f,
			false);
	}
}

void APolyRacingGameModeBase::CheckPlayersAreReady()
{
	if (!ReadyToStartMatch_Implementation())
		return;

	if (SubState == MatchSubState::PreMainEvent)
		SetMatchSubState(MatchSubState::MainEvent);
	else
		SetMatchSubState(MatchSubState::Qualifier);
}

void APolyRacingGameModeBase::CheckPlayersAreFinished()
{
	if (SubState == MatchSubState::Qualifier)
	{
		for (APolyRacingPlayerController* PlayerController : ConnectedPlayers)
		{
			if (PlayerController->GetPlayerState<APolyRacingPlayerState>()->QualifyingTime >= 0)
				return;
		}

		SetMatchSubState(MatchSubState::PostQualifier);
		return;
	}

	for (APolyRacingPlayerController* PlayerController : ConnectedPlayers)
	{
		APolyRacingPlayerState* PlayerState = PlayerController->GetPlayerState<APolyRacingPlayerState>();
		if (PlayerState->EventEndTime < 0)
			return;
	}

	EndMatch();
}

