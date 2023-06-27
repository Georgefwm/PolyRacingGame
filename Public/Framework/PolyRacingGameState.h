// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "PolyRacingGameState.generated.h"


UCLASS()
class POLYRACINGGAME_API APolyRacingGameState : public AGameState
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APolyRacingGameState();

	UPROPERTY()
	int LapCount = 2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
