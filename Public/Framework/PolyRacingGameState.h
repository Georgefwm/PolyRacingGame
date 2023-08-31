// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "PolyRacingGameState.generated.h"


UCLASS()
class POLYRACINGGAME_API APolyRacingGameState : public AGameState
{
	GENERATED_BODY()

public:

	APolyRacingGameState();

	UPROPERTY()
	TArray<class APolyRacingPlayerController*> ConnectedPlayers;

	UPROPERTY()
	int LapCount = 2;

	const TArray<class APolyRacingPlayerController*> GetConnectedPlayers();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
};
