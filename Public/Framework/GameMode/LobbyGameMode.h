﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataTables.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"


USTRUCT()
struct FLobbyPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool bPlayerReadyState = false;

	UPROPERTY()
	FText PlayerName;
};


UCLASS()
class POLYRACINGGAME_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALobbyGameMode();

	TArray<class ALobbyPlayerController*> ConnectedPlayers;
	
	TArray<FLobbyPlayerInfo> ConnectedPlayerInfo;

	UPROPERTY()
	FGameModeTableRow SelectedGameMode;

	UPROPERTY()
	FString NextMap = TEXT("/Game/Scenes/Docks");

	UPROPERTY()
	FTimerHandle GameStartTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void StartPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* ExitingPlayer) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	//called from player controller when he sends a chat message
	void ProdcastChatMessage(const FText& ChatMessage);

	//called from the host to kick a player
	void KickPlayer(int32 PlayerIndex);

	void PlayerRequestUpdate();

	FLobbyPlayerInfo* GetPlayerInfoAtIndex(int Index);
	
	void UpdatePlayerList();

	void StartGameFromLobby();

	void SearchForLobbies();

	bool IsAllPlayerReady() const;

	void CheckLobbyState();

	void DebugPrintConnectedPlayers();
};
