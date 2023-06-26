// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PolyRacingGameModeBase.generated.h"

UCLASS()
class POLYRACINGGAME_API APolyRacingGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APolyRacingGameModeBase();

	UPROPERTY()
	TArray<class APolyRacingPlayerController*> ConnectedPlayers;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSubclassOf<UUserWidget> GameModeWidget;

	FTimerHandle CountDownTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartMatch() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual bool ReadyToStartMatch_Implementation() override;
	
	virtual void CheckIfShouldStart();

	virtual void BeginCountDownSequence();

	virtual void OnCountDownSequenceEnd();
};
