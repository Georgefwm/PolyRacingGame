// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

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

	UFUNCTION(Client, Reliable)
	void ClientShowWidget();

	UFUNCTION(BlueprintCallable, Category = "Networked Sign")
	void ChangeSignText(FText NewText, ANetworkSign* Sign);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeText(const FText & NewText, ANetworkSign* Sign);

	/** Contains the actual implementation of the ServerChangeText function */
	void ServerChangeText_Implementation(const FText & NewText, ANetworkSign* Sign);

	/** Validates the client. If the result is false the client will be disconnected */
	FORCEINLINE bool ServerChangeText_Validate(const FText & NewText, ANetworkSign* Sign) { return true; }
};
