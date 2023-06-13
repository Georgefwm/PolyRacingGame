// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/LobbyGameMode.h"

#include "OnlineSubsystemUtils.h"
#include "PolyRacingSpectatorPawn.h"
#include "Camera/CameraActor.h"
#include "Controller/LobbyPlayerController.h"
#include "Framework/LobbyGameState.h"
#include "Framework/PolyRacingPlayerState.h"
#include "Framework/PolyRacingSessionSubsystem.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MenuHUD.h"


// Sets default values
ALobbyGameMode::ALobbyGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bStartPlayersAsSpectators = true;

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

	UVehicleCustomiser* VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
	
	// Setup the vehicle
	TArray<AActor*> StartPositions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPositions);
	if (!StartPositions.IsEmpty())
	{
		FVector Location = StartPositions[0]->GetTransform().GetLocation();
		FRotator Rotation = StartPositions[0]->GetTransform().GetRotation().Rotator();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		VehicleCustomiser->SpawnVehicle(GetWorld(), Location, Rotation, SpawnParameters);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GAMEMODE: Can't find start position!"))
	}
}

void ALobbyGameMode::StartPlay()
{
	Super::StartPlay();
	
	UPolyRacingSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UPolyRacingSessionSubsystem>();
	SessionSubsystem->FindSessions(10, true);

	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
	
	if (!Cameras.IsEmpty())
	{
		for (ALobbyPlayerController* Player : ConnectedPlayers)
			Player->SetCameraView();
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("LOBBYGAMEMODE: Camera not found"))
}

void ALobbyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
	
	if (!Cameras.IsEmpty())
		NewPlayer->SetViewTarget(StaticCast<ACameraActor*>(Cameras[0]));
	else
		UE_LOG(LogTemp, Warning, TEXT("LOBBYGAMEMODE: Camera not found"))
}

void ALobbyGameMode::InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer)
{
	NewPlayer->ClientSetHUD(HUDClass);

	if (AMenuHUD* HUD = NewPlayer->GetHUD<AMenuHUD>())
	{
		HUD->ShowLobbyMenu();
		UpdatePlayerList();
	}
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

	UE_LOG(LogTemp, Warning, TEXT("get here i guess"))
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// If the joining player is a lobby player controller, add them to a list of connected Players
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
	GetWorld()->ServerTravel(NextMap);
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

