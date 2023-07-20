// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GameMode/PolyRacingGameModeBase.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "CheckpointActor.h"
#include "NetworkMessage.h"
#include "PolyRacingWheeledVehiclePawn.h"
#include "StartPositionActor.h"
#include "Blueprint/UserWidget.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingGameState.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Kismet/GameplayStatics.h"
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

void APolyRacingGameModeBase::SetMatchSubState(FName NewState)
{
	SubState = NewState;
	
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
	SetMatchSubState(SubState);
}

void APolyRacingGameModeBase::HandleQualifierHasStarted()
{
	UE_LOG(LogTemp, Warning, TEXT("MatchSubState changed to: %s"), *SubState.ToString())
	
	BeginCountDownSequence();
}

void APolyRacingGameModeBase::HandleQualifierHasEnded()
{
	UE_LOG(LogTemp, Warning, TEXT("MatchSubState changed to: %s"), *SubState.ToString())
	
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
	UE_LOG(LogTemp, Warning, TEXT("MatchSubState changed to: %s"), *SubState.ToString())
	
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
	
	SetMatchSubState(MatchSubState::MainEvent);
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
	for (const APolyRacingPlayerController* Player : ConnectedPlayers)
	{
		if (!Player->GetPlayerState<APolyRacingPlayerState>()->bIsReady)
			return false;
	}

	return true;
}

bool APolyRacingGameModeBase::ReadyToEndMatch_Implementation()
{
	for (const APolyRacingPlayerController* Player : ConnectedPlayers)
	{
		// Have all players completed the final lap?
		if (Player->GetPlayerState<APolyRacingPlayerState>()->EventEndTime < 0.2f)
			return false;
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
	
	if (ReadyToEndMatch())
	{
		if (SubState == MatchSubState::Qualifier)
			SetMatchSubState(MatchSubState::PostQualifier);
		else
			EndMatch();
	}
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

