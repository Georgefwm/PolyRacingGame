// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/FreeRoamGameMode.h"

#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingPlayerState.h"
#include "UI/InGameHUD.h"


// Sets default values
AFreeRoamGameMode::AFreeRoamGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseSeamlessTravel = false;
	
	PlayerControllerClass = APolyRacingPlayerController::StaticClass();
	HUDClass = AInGameHUD::StaticClass();

	PlayerStateClass = APolyRacingPlayerState::StaticClass();
}

void AFreeRoamGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetGameInstance()->EnableListenServer(true, 7779);
}

void AFreeRoamGameMode::StartPlay()
{
	Super::StartPlay();
}

void AFreeRoamGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	APolyRacingPlayerController* JoiningPlayer = Cast<APolyRacingPlayerController>(NewPlayer);
	if (!JoiningPlayer)
	{
		return;
	}
	
	JoiningPlayer->Client_RequestVehicleSpawn();
}

void AFreeRoamGameMode::InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::InitializeHUDForPlayer_Implementation(NewPlayer);
}

void AFreeRoamGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFreeRoamGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AFreeRoamGameMode::PostLogin(APlayerController* NewPlayer)
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

void AFreeRoamGameMode::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);
}


