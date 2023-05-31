// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/LobbyGameMode.h"


// Sets default values
ALobbyGameMode::ALobbyGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyPlayerController* JoiningPlayer = Cast<ALobbyPlayerController>(NewPlayer);

	//if the joining player is a lobby player controller, add him to a list of connected Players
	if (JoiningPlayer)
		ConnectedPlayers.Add(JoiningPlayer);
}

void ALobbyGameMode::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);
}

void ALobbyGameMode::ProdcastChatMessage(const FText& ChatMessage)
{
}

void ALobbyGameMode::KickPlayer(int32 PlayerIndex)
{
}

void ALobbyGameMode::PlayerRequestUpdate()
{
}

void ALobbyGameMode::UpdatePlayerList()
{
}

void ALobbyGameMode::StartGameFromLobby()
{
}

bool ALobbyGameMode::IsAllPlayerReady() const
{
}

