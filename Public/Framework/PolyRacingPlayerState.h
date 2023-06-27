// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PolyRacingPlayerState.generated.h"

UCLASS()
class POLYRACINGGAME_API APolyRacingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APolyRacingPlayerState();

	UPROPERTY()
	bool bIsReady = true;

	UPROPERTY()
	int LastCheckpoint = -1;

	UPROPERTY()
	int Lap = 0;

	// timestamps using UWorld::GetTimeSeconds
	UPROPERTY()
	TArray<double> LapTimes;

	// timestamp using UWorld::GetTimeSeconds
	UPROPERTY()
	double LastLapStartTime = 0;

	/*
	 * Timestamp using UWorld::GetTimeSeconds
	 * Use this for timing instead of StartTime which is derived from AGameMode
	 */ 
	UPROPERTY()
	double EventStartTime = 0;

	UPROPERTY()
	double EventEndTime = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
