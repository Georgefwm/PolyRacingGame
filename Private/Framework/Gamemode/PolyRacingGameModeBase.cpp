﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/PolyRacingGameModeBase.h"

#include "PolyRacingSpectatorPawn.h"
#include "PolyRacingWheeledVehiclePawn.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingGameState.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Subsystem/GameModeSubsystem.h"
#include "Subsystem/MapSubsystem.h"
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

bool APolyRacingGameModeBase::ReadyToStartMatch_Implementation()
{
	for (APolyRacingPlayerController* Player : ConnectedPlayers)
	{
		if (!Player->GetPlayerState<APolyRacingPlayerState>()->bIsReady)
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
	UE_LOG(LogTemp, Warning, TEXT("Player has finished race"))
	
	// TODO: Set view target as camera watching car go through last checkpoint

	APolyRacingWheeledVehiclePawn* PlayerPawn = PlayerController->GetPawn<APolyRacingWheeledVehiclePawn>();
	if (!PlayerPawn)
		return;

	PlayerController->UnPossess();

	UMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMapSubsystem>();
	 
	if (ULevelSequence* Sequence = MapSubsystem->GetCurrentLevelOutroSequence())
		PlayerController->Client_PlayLevelOutroSequence(Sequence);
	
}

