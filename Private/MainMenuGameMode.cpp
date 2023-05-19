// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

#include "DataTables.h"
#include "EngineUtils.h"
#include "UI/MenuHUD.h"
#include "MenuPlayerController.h"
#include "Customisation/VehicleCustomiser.h"
#include "Engine/DataTable.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = AMenuHUD::StaticClass();

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("DataTable'/Engine/BasicShapes/Cube.Cube'"));
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Setup the menu background vehicle
	FVehicleCustomiser::Initialize();
	TSharedPtr<FVehicleCustomiser> VehicleCustomiser = FVehicleCustomiser::Get();
	
	VehicleCustomiser->PreviewVehicle = GetWorld()->SpawnActor<AVehiclePreview>(AVehiclePreview::StaticClass(),
		FVector(-2020.f,-1700.f,80.f),
		FRotator(0.f, 0.f, 270.f));

	VehicleCustomiser->SetupVehicle();
}
