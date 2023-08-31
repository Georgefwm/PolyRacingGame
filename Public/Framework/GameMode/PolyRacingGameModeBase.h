// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "PolyRacingGameModeBase.generated.h"

class APolyRacingPlayerController;


// Normal gameplay is occurring. Sub-states occur during MatchState::InProgress
namespace MatchSubState
{
	extern POLYRACINGGAME_API const FName Qualifier;
	extern POLYRACINGGAME_API const FName PostQualifier;
	extern POLYRACINGGAME_API const FName PreMainEvent;	
	extern POLYRACINGGAME_API const FName MainEvent;
}

UCLASS()
class POLYRACINGGAME_API APolyRacingGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:

	APolyRacingGameModeBase();
	
	UPROPERTY()
	TArray<class ACheckpointActor*> CheckpointActors;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSubclassOf<UUserWidget> GameModeWidget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSubclassOf<UUserWidget> EndEventWidget;
	
	FTimerHandle CountDownTimerHandle;
	
	UPROPERTY()
	FName SubState = MatchSubState::PreMainEvent;

	// Used for restarting the game
	UPROPERTY()
	FName InitialSubState = MatchSubState::PreMainEvent;

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

	virtual void EndMatch() override;

	virtual void HandleMatchHasEnded() override;

	virtual void CheckPlayersAreReady();

	virtual void CheckPlayersAreFinished();
};