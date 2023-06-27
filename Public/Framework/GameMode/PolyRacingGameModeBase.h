// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "PolyRacingGameModeBase.generated.h"


UCLASS()
class POLYRACINGGAME_API APolyRacingGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:

	APolyRacingGameModeBase();

	UPROPERTY()
	TArray<class APolyRacingPlayerController*> ConnectedPlayers;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSubclassOf<UUserWidget> GameModeWidget;

	FTimerHandle CountDownTimerHandle;

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void StartMatch() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual bool ReadyToStartMatch_Implementation() override;
	
	virtual void CheckIfShouldStart();

	virtual void BeginCountDownSequence();

	virtual void OnCountDownSequenceEnd();

	virtual void HandlePlayerHasFinishedEvent(APolyRacingPlayerController* PlayerController);
	
};