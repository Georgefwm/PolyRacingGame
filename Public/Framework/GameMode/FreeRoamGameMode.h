// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PolyRacingGameModeBase.h"
#include "GameFramework/GameModeBase.h"
#include "FreeRoamGameMode.generated.h"

struct FPresetVehicleConfiguration;

UCLASS()
class POLYRACINGGAME_API AFreeRoamGameMode : public APolyRacingGameModeBase
{
	GENERATED_BODY()

public:
	
	AFreeRoamGameMode();
	
	virtual void BeginPlay() override;

	virtual void StartPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer) override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* ExitingPlayer) override;
	
};
