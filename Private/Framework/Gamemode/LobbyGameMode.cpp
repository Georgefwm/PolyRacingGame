﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/GameMode/LobbyGameMode.h"
#include "Online.h"
#include "PolyRacingSpectatorPawn.h"
#include "Controller/LobbyPlayerController.h"
#include "Framework/LobbyGameState.h"
#include "OnlineSessionSettings.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Framework/PolyRacingSessionSubsystem.h"
#include "Subsystem/MapSubsystem.h"
#include "Subsystem/GameModeSubsystem.h"
#include "UI/MenuHUD.h"


// Sets default values
ALobbyGameMode::ALobbyGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bStartPlayersAsSpectators = true;
	bUseSeamlessTravel = false;

	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	HUDClass = AMenuHUD::StaticClass();
	GameStateClass = ALobbyGameState::StaticClass();
	PlayerStateClass = APolyRacingPlayerState::StaticClass();
	
	DefaultPawnClass = APolyRacingSpectatorPawn::StaticClass();
	SpectatorClass = APolyRacingSpectatorPawn::StaticClass();
}

// Called when the game starts or when spawned
void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetGameInstance()->EnableListenServer(true, 7779);

	// UVehicleCustomiser* VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
	//
	// // Setup the vehicle
	// TArray<AActor*> StartPositions;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPositions);
	// if (!StartPositions.IsEmpty())
	// {
	// 	FVector Location = StartPositions[0]->GetTransform().GetLocation();
	// 	FRotator Rotation = StartPositions[0]->GetTransform().GetRotation().Rotator();
	//
	// 	FActorSpawnParameters SpawnParameters;
	// 	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// 	
	// 	VehicleCustomiser->SpawnVehicle(GetWorld(), Location, Rotation, SpawnParameters);
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("GAMEMODE: Can't find start position!"))
	// }
}

void ALobbyGameMode::StartPlay()
{
	Super::StartPlay();
	
	UPolyRacingSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UPolyRacingSessionSubsystem>();

	FString GameMode;
	SessionSubsystem->LastSessionSettings->Get(SETTING_GAMEMODE, GameMode);

	SessionSubsystem->FindSessions(5, true, GameMode);

	UpdatePlayerList();
	
	UE_LOG(LogTemp, Warning, TEXT("Net mode: %i"), GetWorld()->GetNetMode())

	for (ALobbyPlayerController* Player : ConnectedPlayers)
		Player->SetCameraView();
	
	if (HasAuthority())
		GetWorld()->GetTimerManager().SetTimer(GameStartTimerHandle, this, &ALobbyGameMode::CheckLobbyState, 5.f, false);
}

// Called every frame
void ALobbyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	// Logic for allowing player to connect go here 
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	ALobbyPlayerController* JoiningPlayer = Cast<ALobbyPlayerController>(NewPlayer);
	if (!JoiningPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAMEMODE: invalid player tried to join the lobby"))
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("GAMEMODE: Player joined the lobby"))
	
	ConnectedPlayers.Add(JoiningPlayer);
	
	DebugPrintConnectedPlayers();
}

void ALobbyGameMode::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);

	// Update the ConnectedPlayers Array and the PlayerList in the lobby whenever a player leaves
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(ExitingPlayer);
	if (!LobbyPlayerController)
	{
		return;
	}
	
	ConnectedPlayers.Remove(LobbyPlayerController);
	

	
	PlayerRequestUpdate();
}

void ALobbyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ALobbyPlayerController* JoiningPlayer = Cast<ALobbyPlayerController>(NewPlayer);
	if (!JoiningPlayer)
		return;
	
	JoiningPlayer->Client_SetupHUD(); // Must be client-side
	JoiningPlayer->SetCameraView(); // can be either
	JoiningPlayer->Client_SpawnVehicle(); // Must be client-side
}

void ALobbyGameMode::ProdcastChatMessage(const FText& ChatMessage)
{
	// Call all the connected players and pass in the chat message
	for (ALobbyPlayerController* Player : ConnectedPlayers)
		Player->Client_ReceiveChatMessage(ChatMessage);
}

void ALobbyGameMode::KickPlayer(int32 PlayerIndex)
{
	//call the player to make him destroy his session and leave game
	ConnectedPlayers[PlayerIndex]->Client_GotKicked();
}

void ALobbyGameMode::PlayerRequestUpdate()
{
	UpdatePlayerList();
}

FLobbyPlayerInfo* ALobbyGameMode::GetPlayerInfoAtIndex(int Index)
{
	if (!ConnectedPlayerInfo.IsValidIndex(Index))
		return &ConnectedPlayerInfo.GetData()[Index];
	
	return new FLobbyPlayerInfo();
}

void ALobbyGameMode::UpdatePlayerList()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("GAMEMODE: I dont have auth"))
		return;
	}
	
	// Empty the PlayerInfo Array to re-populate it
	ConnectedPlayerInfo.Empty();

	// Get all the player info from connected players
	for (ALobbyPlayerController* PlayerController : ConnectedPlayers)
	{
		FLobbyPlayerInfo TempLobbyPlayerInfo;

		TempLobbyPlayerInfo.PlayerName = FText::FromString(PlayerController->GetName());
		
		if (APolyRacingPlayerState* PlayerState = Cast<APolyRacingPlayerState>(PlayerController->PlayerState))
			TempLobbyPlayerInfo.bPlayerReadyState = PlayerState->bIsReady;
		else
			TempLobbyPlayerInfo.bPlayerReadyState = false;
		
		ConnectedPlayerInfo.Add(TempLobbyPlayerInfo);
	}

	// call all the players to make them update and pass in the player info array
	for (ALobbyPlayerController* Player : ConnectedPlayers)
		Player->Client_UpdatePlayerList(ConnectedPlayerInfo);
	
}

void ALobbyGameMode::StartGameFromLobby()
{
	
	if (!HasAuthority())
		return;

	UPolyRacingSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UPolyRacingSessionSubsystem>();

	FString GameModeName;
	SessionSubsystem->LastSessionSettings->Get(SETTING_GAMEMODE, GameModeName);

	UMapSubsystem* MapSubsystem = GetGameInstance()->GetSubsystem<UMapSubsystem>();
	UGameModeSubsystem* GameModeSubsystem = GetGameInstance()->GetSubsystem<UGameModeSubsystem>();
	
	FString const LevelOptions = FString("?listen game=" + GameModeSubsystem->GetGameModePath(GameModeName).ToString());
	FString const TravelPath = FString(MapSubsystem->GetMapPath("MainMenu").ToString() + LevelOptions);
	
	GetWorld()->ServerTravel(TravelPath);
}

void ALobbyGameMode::SearchForLobbies()
{
	
}

bool ALobbyGameMode::IsAllPlayerReady() const
{
	for (ALobbyPlayerController* PlayerController : ConnectedPlayers)
	{
		APolyRacingPlayerState* PlayerState = Cast<APolyRacingPlayerState>(PlayerController->PlayerState);
		if (!PlayerState)
			continue;
		
		if (!PlayerState->bIsReady)
			return false;
	}
	
	return true;
}

void ALobbyGameMode::CheckLobbyState()
{
	if (ConnectedPlayers.Num() >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting game in 5..."))
		GetWorld()->GetTimerManager().SetTimer(GameStartTimerHandle, this, &ALobbyGameMode::StartGameFromLobby, 5.f, false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Checking if game is ready to start"))
	GetWorld()->GetTimerManager().SetTimer(GameStartTimerHandle, this, &ALobbyGameMode::CheckLobbyState, 5.f, false);
}

void ALobbyGameMode::DebugPrintConnectedPlayers()
{
	UE_LOG(LogTemp, Warning, TEXT("------------------------------------"));
	UE_LOG(LogTemp, Warning, TEXT("          Current Lobby"));
	UE_LOG(LogTemp, Warning, TEXT("Current Lobby Size: %i/8"), ConnectedPlayers.Num());
	UE_LOG(LogTemp, Warning, TEXT("------------------------------------"));
	for (ALobbyPlayerController* PlayerController : ConnectedPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player: %s"), *PlayerController->GetName());
	}
	UE_LOG(LogTemp, Warning, TEXT("------------------------------------"));
}

