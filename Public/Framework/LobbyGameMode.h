// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FString NextMap = TEXT("/Game/Scenes/Docks");
	
	UPROPERTY()
	ACameraActor* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void StartPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* ExitingPlayer) override;
	
	//called from player controller when he sends a chat message
	void ProdcastChatMessage(const FText& ChatMessage);

	//called from the host to kick a player
	void KickPlayer(int32 PlayerIndex);

	void PlayerRequestUpdate();

	FLobbyPlayerInfo* GetPlayerInfoAtIndex(int Index);
	
	void UpdatePlayerList();

	void StartGameFromLobby();

	bool IsAllPlayerReady() const;

	void DebugPrintConnectedPlayers();
};
