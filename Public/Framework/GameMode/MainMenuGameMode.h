// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"


/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AMainMenuGameMode();

	UPROPERTY()
	ACameraActor* Camera;

	virtual void BeginPlay() override;

	virtual void StartPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer) override;
};
