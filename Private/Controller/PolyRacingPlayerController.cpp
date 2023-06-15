// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PolyRacingPlayerController.h"

#include "UI/InGameHUD.h"

APolyRacingPlayerController::APolyRacingPlayerController()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APolyRacingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
}

void APolyRacingPlayerController::SetupHUD()
{
	if (AInGameHUD* HUD = GetHUD<AInGameHUD>())
	{
		HUD->ShowPlayerHUD();
	}
}

void APolyRacingPlayerController::Client_SetupHUD_Implementation()
{
	SetupHUD();
}

void APolyRacingPlayerController::SpawnVehicleForPlayer(const FPresetVehicleConfiguration& DesiredConfiguration)
{
}

void APolyRacingPlayerController::Server_SpawnVehicleForPlayer_Implementation(const FPresetVehicleConfiguration& DesiredConfiguration)
{
	SpawnVehicleForPlayer(DesiredConfiguration);
}

void APolyRacingPlayerController::RequestVehicleSpawn()
{
	UVehicleCustomiser* VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
	
	Server_SpawnVehicleForPlayer(VehicleCustomiser->SavedConfigurations->GetData()[VehicleCustomiser->ActiveConfigurationSlotIndex]);
}

void APolyRacingPlayerController::Client_RequestVehicleSpawn_Implementation()
{
	RequestVehicleSpawn();
}
