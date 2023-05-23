// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

#include "UI/MenuHUD.h"
#include "MenuPlayerController.h"
#include "Customisation/VehicleCustomiser.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = AMenuHUD::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Setup the menu background vehicle
	FVehicleCustomiser::Initialize();
	TSharedPtr<FVehicleCustomiser> VehicleCustomiser = FVehicleCustomiser::Get();
	
	VehicleCustomiser->PreviewVehicle = GetWorld()->SpawnActor<AVehiclePreview>(AVehiclePreview::StaticClass(),
		FVector(-2020.f, -1700.f, 15.f),
		FRotator(0.f, 90.f, 0.f));

	VehicleCustomiser->SetupVehicle();
}
