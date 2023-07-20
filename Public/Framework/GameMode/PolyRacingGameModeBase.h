// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "PolyRacingGameModeBase.generated.h"


// Normal gameplay is occurring. Sub-states occur during MatchState::InProgress
namespace MatchSubState
{
	POLYRACINGGAME_API const FName Qualifier;
	POLYRACINGGAME_API const FName PostQualifier;
	POLYRACINGGAME_API const FName PreMainEvent;	
	POLYRACINGGAME_API const FName MainEvent;
}

UCLASS()
class POLYRACINGGAME_API APolyRacingGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:

	APolyRacingGameModeBase();

	// TODO: Move this to GameState class
	UPROPERTY()
	TArray<class APolyRacingPlayerController*> ConnectedPlayers;
	
	UPROPERTY()
	TArray<class ACheckpointActor*> CheckpointActors;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSubclassOf<UUserWidget> GameModeWidget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSubclassOf<UUserWidget> EndEventWidget;
	
	FTimerHandle CountDownTimerHandle;

	// Used to set initial state on StartMatch()
	UPROPERTY()
	FName SubState = MatchSubState::MainEvent;

	virtual void SetMatchSubState(FName NewState);

protected:
	
	virtual void BeginPlay() override;

public:

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void RestartPlayer(AController* NewPlayer) override;
	
	virtual void RestartPlayerAtCheckpoint(APolyRacingPlayerController* PlayerController);

	virtual bool ReadyToStartMatch_Implementation() override;
	
	virtual void StartMatch() override;
	
	virtual void HandleQualifierHasStarted();
	
	virtual void HandleQualifierHasEnded();

	virtual void HandleMainEventIsWaitingToStart();
	
	virtual void HandleMainEventHasStarted();
	
	virtual bool ReadyToEndMatch_Implementation() override;

	virtual void BeginCountDownSequence();

	virtual void OnCountDownSequenceEnd();

	virtual void HandlePlayerHasFinishedEvent(APolyRacingPlayerController* PlayerController);

	virtual void HandleMatchHasEnded() override;
	
};