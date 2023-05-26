// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MainMenuGameMode.h"

#include "UI/MenuHUD.h"
#include "Controller/MenuPlayerController.h"
#include "Customisation/VehicleCustomiser.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = AMenuHUD::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup the menu background vehicle
	UVehicleCustomiser* VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
	VehicleCustomiser->SetupVehicle();
}
