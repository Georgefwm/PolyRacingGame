// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PolyRacingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API APolyRacingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	APolyRacingPlayerController();
	
	virtual void BeginPlay() override;

	void SetupHUD();

	UFUNCTION(Client, Reliable)
	void Client_SetupHUD();
	void Client_SetupHUD_Implementation();

	void SpawnVehicleForPlayer(const FPresetVehicleConfiguration& DesiredConfiguration);

	UFUNCTION(Server, Reliable)
	void Server_SpawnVehicleForPlayer(const FPresetVehicleConfiguration& DesiredConfiguration);
	void Server_SpawnVehicleForPlayer_Implementation(const FPresetVehicleConfiguration& DesiredConfiguration);


	void RequestVehicleSpawn();
	
	UFUNCTION(Client, Reliable)
	void Client_RequestVehicleSpawn();
	void Client_RequestVehicleSpawn_Implementation();
};
