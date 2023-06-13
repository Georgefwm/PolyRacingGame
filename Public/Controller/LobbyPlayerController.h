// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

struct FLobbyPlayerInfo;

UCLASS()
class POLYRACINGGAME_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALobbyPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnFinishedLoad();

	// Surely theres a better name...
	TArray<TSharedPtr<FLobbyPlayerInfo>> LobbyPlayerInfoList;
	
	void SendChatMessage(const FText &ChatMessage);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_SendChatMessage(const FText & ChatMessage);
	void Server_SendChatMessage_Implementation(const FText & ChatMessage);
	bool Server_SendChatMessage_Validate(const FText & ChatMessage);

	
	void ReceiveChatMessage(const FText &ChatMessage);
	
	UFUNCTION(Client, Unreliable)
	void Client_ReceiveChatMessage(const FText &ChatMessage);
	void Client_ReceiveChatMessage_Implementation(const FText &ChatMessage);

	void SetupHUD();

	UFUNCTION(Client, Reliable)
	void Client_SetupHUD();
	void Client_SetupHUD_Implementation();


	void KickPlayer(int32 PlayerIndex);
	
	UFUNCTION(Client, Reliable)
	void Client_GotKicked();
	void Client_GotKicked_Implementation();


	void UpdatePlayerList(const TArray<FLobbyPlayerInfo>& PlayerInfoArray);
	
	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerList(const TArray<FLobbyPlayerInfo>& PlayerInfoArray);
	void Client_UpdatePlayerList_Implementation(const TArray<FLobbyPlayerInfo>& PlayerInfoArray);

	
	void RequestServerPlayerListUpdate();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestServerPlayerListUpdate();
	void Server_RequestServerPlayerListUpdate_Implementation();
	bool Server_RequestServerPlayerListUpdate_Validate();
	
	void SetCameraView();
	
	UFUNCTION(Client, Reliable)
	void Client_SetCameraView();
	void Client_SetCameraView_Implementation();
};
