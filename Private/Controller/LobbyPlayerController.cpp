﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/LobbyPlayerController.h"

#include "Camera/CameraActor.h"
#include "Framework/LobbyGameMode.h"
#include "Framework/PolyRacingGameInstance.h"
#include "Framework/PolyRacingPlayerState.h"
#include "UI/MenuHUD.h"


// Sets default values
ALobbyPlayerController::ALobbyPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	LobbyPlayerInfoList = TArray<TSharedPtr<FLobbyPlayerInfo>>();
	
	for (int i = 0; i < 8; i++)
	{
		FLobbyPlayerInfo* TestPlayer = new FLobbyPlayerInfo();
		
		TestPlayer->PlayerName = FText::FromString(TEXT("player " + FString::FromInt(i)));

		TSharedPtr<FLobbyPlayerInfo> PlayerInfo = MakeShareable(TestPlayer);
		LobbyPlayerInfoList.Add(PlayerInfo);
	}

	UE_LOG(LogTemp, Warning, TEXT("LobbyPlayerInfoList initialised"))
}

// Called every frame
void ALobbyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyPlayerController::SendChatMessage(const FText& ChatMessage)
{
	// If this is the server call the game mode to prodcast the Chat Message
	if (!HasAuthority())
		Server_SendChatMessage(ChatMessage);
	
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
		return;
	
	// Add the player's name to the Chat Message then send it to the server
	const FText OutChatMessage = FText::FromString(PlayerState->GetPlayerName() + ": " + ChatMessage.ToString());
	GameMode->ProdcastChatMessage(OutChatMessage);
}

void ALobbyPlayerController::Server_SendChatMessage_Implementation(const FText& ChatMessage)
{
	// If not the server call the server side function to 
	SendChatMessage(ChatMessage);
}

bool ALobbyPlayerController::Server_SendChatMessage_Validate(const FText& ChatMessage)
{
	return true;
}

void ALobbyPlayerController::ReceiveChatMessage(const FText& ChatMessage)
{
	// TODO: Update local chat widget
}

void ALobbyPlayerController::Client_ReceiveChatMessage_Implementation(const FText& ChatMessage)
{
	// Call the Receive message function to show it on UMG
	ReceiveChatMessage(ChatMessage);
}

void ALobbyPlayerController::KickPlayer(int32 PlayerIndex)
{
	//if the player is the host, get the game mode and send it to kick the player from the game
	if (!HasAuthority())
		return;
	
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
		return;
		
	GameMode->KickPlayer(PlayerIndex);
}

void ALobbyPlayerController::Client_GotKicked_Implementation()
{
	// get the game Instance to make the player destroy his session and leave game
	UPolyRacingGameInstance* GameInstance = Cast<UPolyRacingGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance)
		return;
	
	//show the player that he got kicked in message in UMG
	//GameInstance->ShowErrorMessage(FText::FromString("You got kicked from the server"));
	
	//make the player call the game Instance to destroy his session
	//GameInstance->DestroySessionAndLeaveGame();
}

void ALobbyPlayerController::UpdatePlayerList(const TArray<FLobbyPlayerInfo>& PlayerInfoArray)
{
	if (AMenuHUD* MenuHUD = static_cast<AMenuHUD*>(GetHUD()))
	{
		
	}
}

void ALobbyPlayerController::Client_UpdatePlayerList_Implementation(const TArray<FLobbyPlayerInfo>& PlayerInfoArray)
{
	UpdatePlayerList(PlayerInfoArray);
}

void ALobbyPlayerController::RequestServerPlayerListUpdate()
{
	// If this is the server call the game mode to request info
	if (!HasAuthority())
	{
		Server_RequestServerPlayerListUpdate();
		return;
	}
	
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
		return;
	
	GameMode->PlayerRequestUpdate();
}

void ALobbyPlayerController::Server_RequestServerPlayerListUpdate_Implementation()
{
	// If not the server call the server side function
	RequestServerPlayerListUpdate();
}

bool ALobbyPlayerController::Server_RequestServerPlayerListUpdate_Validate()
{
	return true;
}

void ALobbyPlayerController::SetCameraView()
{
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
		return;

	SetViewTarget(GameMode->Camera);
}

void ALobbyPlayerController::Client_SetCameraView_Implementation()
{
	SetCameraView();
}




