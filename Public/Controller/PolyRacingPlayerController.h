// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTables.h"
#include "GameFramework/PlayerController.h"
#include "PolyRacingPlayerController.generated.h"

class ULevelSequencePlayer;

/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API APolyRacingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	APolyRacingPlayerController();

	UPROPERTY(Replicated)
	FPresetVehicleConfiguration VehicleConfiguration;

	UPROPERTY(Replicated)
	APolyRacingWheeledVehiclePawn* VehiclePawn;

	UPROPERTY()
	ULevelSequencePlayer* SequencePlayer;

	UPROPERTY()
	FTimerHandle SequenceTimerHandle;
	
	virtual void BeginPlay() override;
	

	void SetupHUD();

	UFUNCTION(Client, Reliable)
	void Client_SetupHUD();
	void Client_SetupHUD_Implementation();
	

	void SpawnVehicleForPlayer(const FPresetVehicleConfiguration& DesiredConfiguration, APolyRacingPlayerController* PlayerController);

	UFUNCTION(Server, Reliable)
	void Server_SpawnVehicleForPlayer(const FPresetVehicleConfiguration& DesiredConfiguration, APolyRacingPlayerController* PlayerController);
	void Server_SpawnVehicleForPlayer_Implementation(const FPresetVehicleConfiguration& DesiredConfiguration, APolyRacingPlayerController* PlayerController);
	

	void RequestVehicleSpawn();
	
	UFUNCTION(Client, Reliable)
	void Client_RequestVehicleSpawn();
	void Client_RequestVehicleSpawn_Implementation();

	void PlayLevelIntroSequence(ULevelSequence* Sequence);
	void OnLevelIntroSequenceEnd();

	UFUNCTION(Client, Reliable)
	void Client_PlayLevelIntroSequence(ULevelSequence* Sequence);
	void Client_PlayLevelIntroSequence_Implementation(ULevelSequence* Sequence);
};
