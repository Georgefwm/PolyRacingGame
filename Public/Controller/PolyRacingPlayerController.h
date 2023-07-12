// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataTables.h"
#include "GameFramework/PlayerController.h"
#include "PolyRacingPlayerController.generated.h"

class UInputAction;
class UUserWidget;
class UInputMappingContext;
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

	virtual void SetupInputComponent() override;

	void SetupInputMappingContext();

	UPROPERTY()
	UInputMappingContext* InputMappingContext;

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
	

	void OnLevelIntroSequenceEnd();
	void PlayLevelIntroSequence(ULevelSequence* Sequence);

	UFUNCTION(Client, Reliable)
	void Client_PlayLevelIntroSequence(ULevelSequence* Sequence);
	void Client_PlayLevelIntroSequence_Implementation(ULevelSequence* Sequence);

	
	void OnLevelOutroSequenceEnd();
	void PlayLevelOutroSequence(ULevelSequence* Sequence);

	UFUNCTION(Client, Reliable)
	void Client_PlayLevelOutroSequence(ULevelSequence* Sequence);
	void Client_PlayLevelOutroSequence_Implementation(ULevelSequence* Sequence);

	
	void NotifyReadyToStart();
	
	UFUNCTION(Server, Reliable)
	void Server_NotifyReadyToStart();
	void Server_NotifyReadyToStart_Implementation();

	
	void PlayCountDown();

	UFUNCTION(Client, Reliable)
	void Client_PlayCountDown();
	void Client_PlayCountDown_Implementation();

	
	void OnCountDownSequenceEnd();

	UFUNCTION(Client, Reliable)
	void Client_OnCountDownSequenceEnd();
	void Client_OnCountDownSequenceEnd_Implementation();

	
	void SetGameMode(const FString& GameModeName);

	UFUNCTION(Client, Reliable)
	void Client_SetGameMode(const FString& GameModeName);
	void Client_SetGameMode_Implementation(const FString& GameModeName);

	
	void NotifyFinishedRace(APolyRacingPlayerController* PlayerController);
	
	UFUNCTION(Server, Reliable)
	void Server_NotifyFinishedRace(APolyRacingPlayerController* PlayerController);
	void Server_NotifyFinishedRace_Implementation(APolyRacingPlayerController* PlayerController);

	
	void RequestCheckpointRestart(APolyRacingPlayerController* PlayerController);
	
	UFUNCTION(Server, Reliable)
	void Server_RequestCheckpointRestart(APolyRacingPlayerController* PlayerController);
	void Server_RequestCheckpointRestart_Implementation(APolyRacingPlayerController* PlayerController);

	
	void AddWidgetToScreen(TSubclassOf<UUserWidget> Widget);

	UFUNCTION(Client, Reliable)
	void Client_AddWidgetToScreen(TSubclassOf<UUserWidget> Widget);
	void Client_AddWidgetToScreen_Implementation(TSubclassOf<UUserWidget> Widget);

	// Fades camera to black then calls LeaveMatchSinglePlayer()
	void StartLeavingMatchSinglePlayer();

	// Instantly leave current single-player match
	void LeaveMatchSinglePlayer();

	// Fades camera to black then calls LeaveMatchMultiPlayer()
	void StartLeavingMatchMultiPlayer();

	// Instantly leave current multi-player match
	void LeaveMatchMultiPlayer();
};
