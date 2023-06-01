// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/LobbyGameMode.h"

#include "PolyRacingSpectatorPawn.h"
#include "Camera/CameraActor.h"
#include "Controller/LobbyPlayerController.h"
#include "Framework/PolyRacingPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/LobbyHUD.h"


// Sets default values
ALobbyGameMode::ALobbyGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bStartPlayersAsSpectators = true;

	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	HUDClass = ALobbyHUD::StaticClass();
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

	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);
	
	if (!Cameras.IsEmpty())
		Camera = StaticCast<ACameraActor*>(Cameras[0]);
		
	for (ALobbyPlayerController* Player : ConnectedPlayers)
	{
		Player->Client_SetCameraView();
	}
}

// Called every frame
void ALobbyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// If the joining player is a lobby player controller, add him to a list of connected Players
	ALobbyPlayerController* JoiningPlayer = Cast<ALobbyPlayerController>(NewPlayer);
	if (!JoiningPlayer)
		return;
	
	ConnectedPlayers.Add(JoiningPlayer);
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
	UpdatePlayerList();
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
}

FLobbyPlayerInfo ALobbyGameMode::GetPlayerInfoAtIndex(int Index)
{
	if (!ConnectedPlayerInfo.IsValidIndex(Index))
		return ConnectedPlayerInfo.GetData()[Index];
	
	return FLobbyPlayerInfo();
}

void ALobbyGameMode::UpdatePlayerList()
{
	// Empty the PlayerInfo Array to re-populate it
	ConnectedPlayerInfo.Empty();

	// Get all the player info from connected players
	for (ALobbyPlayerController* PlayerController : ConnectedPlayers)
	{
		FLobbyPlayerInfo TempLobbyPlayerInfo;

		APolyRacingPlayerState* PlayerState = Cast<APolyRacingPlayerState>(PlayerController->PlayerState);
		if (PlayerState)
			TempLobbyPlayerInfo.bPlayerReadyState = PlayerState->bIsReady;
		else
			TempLobbyPlayerInfo.bPlayerReadyState = false;

		TempLobbyPlayerInfo.PlayerName = PlayerController->PlayerState->GetPlayerName();
		ConnectedPlayerInfo.Add(TempLobbyPlayerInfo);
	}

	//call all the players to make them update and pass in the player info array
	for (ALobbyPlayerController* Player : ConnectedPlayers)
		Player->Client_UpdatePlayerList(ConnectedPlayerInfo);
}

void ALobbyGameMode::StartGameFromLobby()
{
	//GetWorld()->ServerTravel(GameMapName);
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

