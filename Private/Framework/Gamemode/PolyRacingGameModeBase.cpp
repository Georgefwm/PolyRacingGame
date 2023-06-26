// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameMode/PolyRacingGameModeBase.h"

#include "PolyRacingWheeledVehiclePawn.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Framework/PolyRacingPlayerState.h"
#include "UI/InGameHUD.h"

// Sets default values
APolyRacingGameModeBase::APolyRacingGameModeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APolyRacingGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APolyRacingGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	UE_LOG(LogTemp, Warning, TEXT("Checking if game should start"))

	if (MatchState == MatchState::InProgress)
		return;
	
	if (ReadyToStartMatch())
	{
		UE_LOG(LogTemp, Warning, TEXT("Players are ready, starting game"))
		StartMatch();
	}
}

void APolyRacingGameModeBase::BeginCountDownSequence()
{
	for (APolyRacingPlayerController* Player : ConnectedPlayers)
	{
		Player->Client_PlayCountDown();

		if (APolyRacingWheeledVehiclePawn* VehiclePawn = Player->GetPawn<APolyRacingWheeledVehiclePawn>())
			VehiclePawn->DisableInput(Player);
	}

	// @ASSUMPTION : Count down animation sequence is 4 seconds long
	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &APolyRacingGameModeBase::OnCountDownSequenceEnd, 4.f, false);
}

void APolyRacingGameModeBase::OnCountDownSequenceEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Enabling player input"))

	for (APolyRacingPlayerController* Player : ConnectedPlayers)
	{
		if (APolyRacingWheeledVehiclePawn* VehiclePawn = Player->GetPawn<APolyRacingWheeledVehiclePawn>())
			VehiclePawn->EnableInput(Player);
	}
}

